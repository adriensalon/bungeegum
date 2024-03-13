#pragma once

#include <bungeegum/config/feature.hpp>
#include <bungeegum/glue/serialize.hpp>

#if BUNGEEGUM_USE_HOTSWAP

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#define HSCPP_CXX_STANDARD 17
#include <hscpp/mem/MemoryManager.h>
#include <hscpp/mem/Ref.h>
#include <hscpp/module/Tracker.h>

#include <bungeegum/core/math.hpp>
#include <bungeegum/glue/detection.hpp>
#include <bungeegum/glue/foreach.hpp>

/// @brief Implements hotswapping functionnality to structs that use this macro among public or
/// private fields. Variadic parameters are fields of this struct that we want to be serialized
/// between recompilations.
#define HOTSWAP_CLASS(classname, ...)                                                                                                                                                               \
    friend class cereal::access;                                                                                                                                                                    \
    friend struct bungeegum::access;                                                                                                                                                                    \
    friend struct bungeegum::detail::swapped_access;                                                                                                                                              \
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
    hscpp_virtual void load(cereal::JSONInputArchive& archive)                                                                                                                           \
    {                                                                                                                                                                                               \
        bungeegum::detail::serialize_fields(archive, std::string("BUNGEEGUM_OBJECT_REFERENCE, ") + std::string(#__VA_ARGS__), _bungeegum_object_reference, __VA_ARGS__);  \
    }                                                                                                                                                                                               \
    hscpp_virtual void save(cereal::JSONOutputArchive& archive) const                                                                                                                         \
    {                                                                                                                                                                                               \
        bungeegum::detail::serialize_fields(archive, std::string("BUNGEEGUM_OBJECT_REFERENCE, ") + std::string(#__VA_ARGS__), _bungeegum_object_reference, __VA_ARGS__); \
    }

/// @brief Implements hotswapping functionnality to methods of struct that already use the
/// HOTSWAP_CLASS macro. As these methods need to be virtual to be hotswapped they can not be
/// templated.
#define HOTSWAP_METHOD hscpp_virtual

/// @cond DO_NOT_DOCUMENT
namespace hscpp {
class Hotswapper;
namespace mem {
    class MemoryManager;
}
}
/// @endcond

namespace bungeegum {
namespace detail {    

    template <typename value_t>
    struct swapped;

    namespace traits {
        
        template <typename value_t>
        using detected_this_function = decltype(std::declval<value_t>()._bungeegum_this());

        template <typename value_t>
        using detected_sizeof_function = decltype(std::declval<value_t>()._bungeegum_sizeof());

        template <typename value_t>
        constexpr bool has_this_function_v = detail::is_detected_exact_v<std::uintptr_t, detected_this_function, value_t>;

        template <typename value_t>
        constexpr bool has_sizeof_function_v = detail::is_detected_exact_v<std::size_t, detected_sizeof_function, value_t>;

        /// @brief True if value_t uses the HOTSWAP_CLASS macro.
        template <typename value_t>
        constexpr bool is_reloadable_v = (has_this_function_v<value_t> && has_sizeof_function_v<value_t>);
    }

    template <typename value_t, typename = void>
    struct value_type {
        using type = value_t;
    };

    template <typename value_t>
    struct value_type<value_t, std::enable_if_t<traits::is_reloadable_v<value_t>>> {
        using type = swapped<value_t>;
    };

    template <typename value_t, typename = void>
    struct reference_type {
        using type = std::reference_wrapper<value_t>;
    };

    template <typename value_t>
    struct reference_type<value_t, std::enable_if_t<traits::is_reloadable_v<value_t>>> {
        using type = swapped<value_t>;
    };

    /// @brief Equals to reloaded<value_t> if value_t is reloadable, and to value_t otherwise.
    template <typename value_t>
    using value_type_t = typename value_type<value_t>::type;

    /// @brief Equals to reloaded<value_t> if value_t is reloadable, and to value_t& otherwise.
    template <typename value_t>
    using reference_type_t = typename reference_type<value_t>::type;


    /// @brief 
    struct swapped_access {

        /// @brief 
        /// @tparam swapped_t 
        /// @param object 
        /// @return 
        template <typename swapped_t>
        [[nodiscard]] static std::uintptr_t& get_object_reference(swapped_t& object);

        /// @brief 
        /// @tparam swapped_t 
        /// @param object 
        /// @return 
        template <typename swapped_t>
        [[nodiscard]] static std::uintptr_t get_this(swapped_t& object);

        /// @brief 
        /// @tparam swapped_t 
        /// @param object 
        /// @return 
        template <typename swapped_t>
        [[nodiscard]] static std::size_t get_sizeof(swapped_t& object);
    };

    /// @brief Instances of this struct wrap values so they can be accessed between recompilations
    /// and method calls can be updated too.
    /// @details Instances of this struct can be both copied (shallow copy) and moved.
    /// @tparam value_t is the type to use as value.
    template <typename value_t>
    struct swapped {
        swapped();
        swapped(const swapped& other);
        swapped& operator=(const swapped& other);
        swapped(swapped&& other);
        swapped& operator=(swapped&& other);

        /// @brief Gets a non-const reference to the underlying value.
        [[nodiscard]] value_t& get() const;

    private:
        hscpp::mem::UniqueRef<value_t> _ref;
        swapped(hscpp::mem::UniqueRef<value_t>&& ref);
        friend struct swapper_handle;
    };

    /// @brief Current state of the hotswapping.
    enum struct swapper_state {
        idle,
        compiling,
        started_compiling,
        performed_swap,
        failed_swap,
    };

    /// @brief Instances of this struct allocate memory that can be hotswapped. They trigger
    /// compilation and manage the files and directories to provide to the compiler.
    /// @details Instances of this struct can only be moved.
    struct swapper_handle {
        swapper_handle() = delete;
        swapper_handle(const swapper_handle& other) = delete;
        swapper_handle& operator=(const swapper_handle& other) = delete;
        swapper_handle(swapper_handle&& other) = default;
        swapper_handle& operator=(swapper_handle&& other) = default;

        /// @brief Creates an instance from a std::wstreambuf and writes an initialization
        /// message to it.
        swapper_handle(std::wstreambuf* buffer);

        /// @brief Allocates a new object that can be hotswapped, taking no argument.
        /// @exception Throws a compile-time exception if the swapped type is not default-
        /// constructible.
        template <typename value_t>
        [[nodiscard]] swapped<value_t> allocate();

        /// @brief Gets a modifiable list of the preprocessor definitions for recompilation.
        [[nodiscard]] std::vector<std::string>& defines();

        /// @brief Gets a modifiable list of the include directories for recompilation.
        [[nodiscard]] std::vector<std::filesystem::path>& include_directories();

        /// @brief Gets a modifiable list of the source directories for recompilation.
        [[nodiscard]] std::vector<std::filesystem::path>& source_directories();

        /// @brief Gets a modifiable list of the force compiled source files for recompilation.
        [[nodiscard]] std::vector<std::filesystem::path>& force_compiled_source_files();

        /// @brief Gets a modifiable list of the libraries for recompilation.
        [[nodiscard]] std::vector<std::filesystem::path>& libraries();

        /// @brief Gets the amount of memory blocks currently allocated.
        [[nodiscard]] std::size_t allocated_blocks_count();

        /// @brief Forces an update, blocking the thread until recompilation has finished.
        void force_update();

        /// @brief Asynchronously waits for changes in the provided files and folders, triggers
        /// recompilation when needed and hotswaps allocated objects. It takes a std::wstreambuf
        /// to be filled with what would go to std::wcout otherwise.
        swapper_state update(std::wstreambuf* buffer);

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
		std::vector<std::string> _logs = {};
    };

    /// @brief Instances of this struct represent a cereal JSON input archive to load data agter
    /// recompilation.
    /// @details Instances of this struct can only be moved.
    struct swapped_load_guard {
        swapped_load_guard() = delete;
        swapped_load_guard(const swapped_load_guard& other) = delete;
        swapped_load_guard& operator=(const swapped_load_guard& other) = delete;
        swapped_load_guard(swapped_load_guard&& other) = delete;
        swapped_load_guard& operator=(swapped_load_guard&& other) = delete;

        /// @brief Creates an instance of this struct from a file path.
        swapped_load_guard(const std::filesystem::path& archive_path);

        /// @brief Creates an instance of this struct from a stringstream.
        swapped_load_guard(std::stringstream& archive_stream);

        /// @brief Loads a reloaded value. Can be used multiple times to load many values from
        /// the same file.
        /// @tparam value_t is the type to use as value.
        template <typename value_t>
        void load(const swapped<value_t>& value);

    private:
        std::optional<std::reference_wrapper<std::stringstream>> _sstream = std::nullopt;
        std::optional<std::ifstream> _fstream = std::nullopt;
        cereal::JSONInputArchive _archive;
    };

    /// @brief Instances of this struct represent a cereal JSON output archive to save data after
    /// recompilation.
    /// @details Instances of this struct can only be moved.
    struct swapped_save_guard {
        swapped_save_guard() = delete;
        swapped_save_guard(const swapped_save_guard& other) = delete;
        swapped_save_guard& operator=(const swapped_save_guard& other) = delete;
        swapped_save_guard(swapped_save_guard&& other) = delete;
        swapped_save_guard& operator=(swapped_save_guard&& other) = delete;

        /// @brief Creates an instance of this struct from a file path.
        swapped_save_guard(const std::filesystem::path& archive_path);

        /// @brief Creates an instance of this struct from a stringstream.
        swapped_save_guard(std::stringstream& archive_stream);

        /// @brief Saves a reloaded value. Can be used multiple times to save many values to the
        /// same file.
        /// @tparam value_t is the type to use as value.
        template <typename value_t>
        void save(const swapped<value_t>& value);

    private:
        std::optional<std::reference_wrapper<std::stringstream>> _sstream = std::nullopt;
        std::optional<std::ofstream> _fstream = std::nullopt;
        cereal::JSONOutputArchive _archive;
    };

    /// @brief Gets
    /// @tparam value_t
    template <typename value_t>
    [[nodiscard]] value_t& get_global_data();
    
}
}

#include <bungeegum/glue/hotswap.inl>

#else

/// @brief Only serializes fields for overlay. Define BUNGEEGUM_ENABLE_HOTSWAP to 1 to enable.
#define HOTSWAP_CLASS(classname, ...)
// #define HOTSWAP_CLASS(classname, ...) SERIALIZE_FIELDS(__VA_ARGS__)

/// @brief Does nothing as hotswap is unused. Define BUNGEEGUM_ENABLE_HOTSWAP to 1 to enable.
#define HOTSWAP_METHOD

namespace bungeegum {
namespace detail {

    template <typename value_t>
    using value_type_t = value_t;

    template <typename value_t>
    using reference_type_t = std::reference_wrapper<value_t>;
}
}

#endif