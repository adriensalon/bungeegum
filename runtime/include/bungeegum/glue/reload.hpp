#pragma once

#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/toolchain.hpp>

/// @brief Defines if we use reload functionnality.
#if !defined(BUNGEEGUM_USE_HOTSWAP)
#define BUNGEEGUM_USE_HOTSWAP (BUNGEEGUM_ENABLE_HOTSWAP && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX))
#endif

#if BUNGEEGUM_USE_HOTSWAP

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#define HSCPP_CXX_STANDARD 17
#include <hscpp/mem/Ref.h>
#include <hscpp/module/Tracker.h>

#include <bungeegum/glue/detection.hpp>
#include <bungeegum/glue/foreach.hpp>
#include <bungeegum/glue/serialize.hpp>
#include <bungeegum/glue/simd.hpp>

/// @brief Implements hotswapping functionnality to structs that use this macro among public or
/// private fields. Variadic parameters are fields of this struct that we want to be serialized
/// between recompilations.
#define HOTSWAP_CLASS(classname, ...)                                                                                                                                                               \
    friend class cereal::access;                                                                                                                                                                    \
    friend struct bungeegum::access;                                                                                                                                                                \
    template <typename value_t>                                                                                                                                                                     \
    friend struct bungeegum::detail::reloaded_wrapper;                                                                                                                                              \
    friend struct bungeegum::detail::widgets_manager;                                                                                                                                               \
    HSCPP_TRACK(classname, #classname)                                                                                                                                                              \
    std::uintptr_t _bungeegum_object_reference = 0;                                                                                                                                                 \
    hscpp_virtual std::uintptr_t _bungeegum_this()                                                                                                                                                  \
    {                                                                                                                                                                                               \
        return bungeegum::detail::raw_cast<classname>(this);                                                                                                                                        \
    }                                                                                                                                                                                               \
    hscpp_virtual std::size_t _bungeegum_sizeof()                                                                                                                                                   \
    {                                                                                                                                                                                               \
        return sizeof(classname);                                                                                                                                                                   \
    }                                                                                                                                                                                               \
    hscpp_virtual void _bungeegum_load(cereal::JSONInputArchive& archive)                                                                                                                           \
    {                                                                                                                                                                                               \
        bungeegum::detail::serialize_fields<cereal::JSONInputArchive>(archive, std::string("BUNGEEGUM_OBJECT_REFERENCE, ") + std::string(#__VA_ARGS__), _bungeegum_object_reference, __VA_ARGS__);  \
    }                                                                                                                                                                                               \
    hscpp_virtual void _bungeegum_save(cereal::JSONOutputArchive& archive)                                                                                                                          \
    {                                                                                                                                                                                               \
        bungeegum::detail::serialize_fields<cereal::JSONOutputArchive>(archive, std::string("BUNGEEGUM_OBJECT_REFERENCE, ") + std::string(#__VA_ARGS__), _bungeegum_object_reference, __VA_ARGS__); \
    }

/// @brief Implements hotswapping functionnality to methods of struct that already use the
/// HOTSWAP_CLASS macro. As these methods need to be virtual to be hotswapped they can not be
/// templated.
#define HOTSWAP_METHOD hscpp_virtual

namespace hscpp {
class Hotswapper;
namespace mem {
    class MemoryManager;
}
}

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct wrap values so they can be accessed between recompilations
    /// and method calls can be updated too.
    /// @details Instances of this struct can be both copied (shallow copy) and moved.
    /// @tparam value_t is the type to use as value.
    template <typename value_t>
    struct reloaded {
        reloaded();
        reloaded(const reloaded& other);
        reloaded& operator=(const reloaded& other);
        reloaded(reloaded&& other);
        reloaded& operator=(reloaded&& other);

        /// @brief Gets a non-const reference to the underlying value.
        value_t& get() const;

    private:
        hscpp::mem::UniqueRef<value_t> _ref;
        reloaded(hscpp::mem::UniqueRef<value_t>&& ref);
        friend struct reloader;
    };

    /// @brief Current state of the hotswapping.
    enum struct reload_state {
        idle,
        compiling,
        started_compiling,
        performed_swap,
        failed_swap,
    };

    /// @brief Instances of this struct allocate memory that can be hotswapped. They trigger
    /// compilation and manage the files and directories to provide to the compiler.
    /// @details Instances of this struct can only be moved.
    struct reloader {
        reloader() = delete;
        reloader(const reloader& other) = delete;
        reloader& operator=(const reloader& other) = delete;
        reloader(reloader&& other) = default;
        reloader& operator=(reloader&& other) = default;

        /// @brief Creates an instance from a std::wstreambuf and writes an initialization
        /// message to it.
        reloader(std::wstreambuf* buffer);

        /// @brief Allocates a new object that can be hotswapped, taking no argument.
        /// @exception Throws a compile-time exception if the widget type is not default-
        /// constructible.
        template <typename value_t>
        reloaded<value_t> allocate();

        /// @brief Gets a modifiable list of the preprocessor definitions for recompilation.
        std::vector<std::string>& defines();

        /// @brief Gets a modifiable list of the include directories for recompilation.
        std::vector<std::filesystem::path>& include_directories();

        /// @brief Gets a modifiable list of the source directories for recompilation.
        std::vector<std::filesystem::path>& source_directories();

        /// @brief Gets a modifiable list of the force compiled source files for recompilation.
        std::vector<std::filesystem::path>& force_compiled_source_files();

        /// @brief Gets a modifiable list of the libraries for recompilation.
        std::vector<std::filesystem::path>& libraries();

        /// @brief Gets the amount of memory blocks currently allocated.
        std::size_t allocated_blocks_count();

        /// @brief Forces an update, blocking the thread until recompilation has finished.
        void force_update();

        /// @brief Asynchronously waits for changes in the provided files and folders, triggers
        /// recompilation when needed and hotswaps allocated objects. It takes a std::wstreambuf
        /// to be filled with what would go to std::wcout otherwise.
        reload_state update(std::wstreambuf* buffer);

        /// @brief Sets an instance of any object as the global data. This instance will be shared
        /// with the hotswapped modules, and can be accessed anywhere with the get_global_data()
        /// function.
        void set_global_data(void* data_ptr);

    private:
        std::pair<std::size_t, std::vector<std::string>> _defines = { false, {} };
        std::pair<std::size_t, std::vector<std::filesystem::path>> _include_directories = { false, {} };
        std::pair<std::size_t, std::vector<std::filesystem::path>> _source_directories = { false, {} };
        std::pair<std::size_t, std::vector<std::filesystem::path>> _force_compiled_source_files = { false, {} };
        std::pair<std::size_t, std::vector<std::filesystem::path>> _libraries = { false, {} };
        std::shared_ptr<hscpp::Hotswapper> _swapper = nullptr;
        std::shared_ptr<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>> _manager = nullptr;
    };

    /// @brief Gets
    /// @tparam value_t
    template <typename value_t>
    value_t& get_global_data();

    /// @brief Instances of this struct represent a cereal JSON input archive to load data agter
    /// recompilation.
    /// @details Instances of this struct can only be moved.
    struct reloaded_loader {
        reloaded_loader() = delete;
        reloaded_loader(const reloaded_loader& other) = delete;
        reloaded_loader& operator=(const reloaded_loader& other) = delete;
        reloaded_loader(reloaded_loader&& other) = default;
        reloaded_loader& operator=(reloaded_loader&& other) = default;

        /// @brief Creates an instance of this struct from a file path.
        reloaded_loader(const std::filesystem::path& archive_path);

        /// @brief Creates an instance of this struct from a stringstream.
        reloaded_loader(std::stringstream& archive_stream);

        /// @brief Loads a reloaded value. Can be used multiple times to load many values from
        /// the same file.
        /// @tparam value_t is the type to use as value.
        template <typename value_t>
        void load(reloaded<value_t>& value);

    private:
        std::optional<std::reference_wrapper<std::stringstream>> _sstream = std::nullopt;
        std::optional<std::ifstream> _fstream = std::nullopt;
        cereal::JSONInputArchive _archive;
    };

    /// @brief Instances of this struct represent a cereal JSON output archive to save data after
    /// recompilation.
    /// @details Instances of this struct can only be moved.
    struct reloaded_saver {
        reloaded_saver() = delete;
        reloaded_saver(const reloaded_saver& other) = delete;
        reloaded_saver& operator=(const reloaded_saver& other) = delete;
        reloaded_saver(reloaded_saver&& other) = default;
        reloaded_saver& operator=(reloaded_saver&& other) = default;

        /// @brief Creates an instance of this struct from a file path.
        reloaded_saver(const std::filesystem::path& archive_path);

        /// @brief Creates an instance of this struct from a stringstream.
        reloaded_saver(std::stringstream& archive_stream);

        /// @brief Saves a reloaded value. Can be used multiple times to save many values to the
        /// same file.
        /// @tparam value_t is the type to use as value.
        template <typename value_t>
        void save(reloaded<value_t>& value);

    private:
        std::optional<std::reference_wrapper<std::stringstream>> _sstream = std::nullopt;
        std::optional<std::ofstream> _fstream = std::nullopt;
        cereal::JSONOutputArchive _archive;
    };
}
}

#else

/// @brief Only serializes fields for overlay. Define BUNGEEGUM_ENABLE_HOTSWAP to 1 to enable.
#define HOTRELOAD_CLASS(classname, ...) SERIALIZE_FIELDS(__VA_ARGS__)

/// @brief Does nothing as hotswap is unused. Define BUNGEEGUM_ENABLE_HOTSWAP to 1 to enable.
#define HOTRELOAD_METHOD

#endif

namespace bungeegum {
namespace detail {

    namespace traits {

        template <typename value_t>
        using detected_load_function = decltype(std::declval<value_t>()._bungeegum_load(std::declval<cereal::JSONInputArchive&>()));

        template <typename value_t>
        using detected_save_function = decltype(std::declval<value_t>()._bungeegum_save(std::declval<cereal::JSONOutputArchive&>()));

        template <typename value_t>
        constexpr bool has_load_function_v = detail::is_detected_exact_v<void, detected_load_function, value_t>;

        template <typename value_t>
        constexpr bool has_save_function_v = detail::is_detected_exact_v<void, detected_save_function, value_t>;

        /// @brief True if value_t uses the HOTSWAP_CLASS macro.
        template <typename value_t>
        constexpr bool is_reloadable_v = (has_load_function_v<value_t> && has_save_function_v<value_t>);

    }

    template <typename value_t, typename = void>
    struct value_type {
        using type = value_t;
    };

    template <typename value_t>
    struct value_type<value_t, std::enable_if_t<traits::is_reloadable_v<value_t>>> {
        using type = reloaded<value_t>;
    };

    template <typename value_t, typename = void>
    struct reference_type {
        using type = std::reference_wrapper<value_t>;
    };

    template <typename value_t>
    struct reference_type<value_t, std::enable_if_t<traits::is_reloadable_v<value_t>>> {
        using type = reloaded<value_t>;
    };

    /// @brief Equals to reloaded<value_t> if value_t is reloadable, and to value_t otherwise.
    template <typename value_t>
    using value_type_t = typename value_type<value_t>::type;

    /// @brief Equals to reloaded<value_t> if value_t is reloadable, and to value_t& otherwise.
    template <typename value_t>
    using reference_type_t = typename reference_type<value_t>::type;
}
}

#include <bungeegum/glue/reload.inl>