#pragma once

#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/toolchain.hpp>

/// @brief Defines if we use hotreload.
#if !defined(BUNGEEGUM_USE_HOTRELOAD)
#define BUNGEEGUM_USE_HOTRELOAD (BUNGEEGUM_ENABLE_HOTRELOAD && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX))
#endif

#if BUNGEEGUM_USE_HOTRELOAD

#include <cstddef>
#include <filesystem>
#include <fstream>

#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

#define HSCPP_CXX_STANDARD 17
#include <hscpp/mem/Ref.h>
#include <hscpp/module/Tracker.h>

#include <bungeegum/glue/detection.hpp>
#include <bungeegum/glue/foreach.hpp>
#include <bungeegum/glue/simd.hpp>

namespace hscpp {
class Hotswapper;
namespace mem {
    class MemoryManager;
}
}

/// @brief
/// @details
#define HOTRELOAD_CLASS(classname, ...)                                                                                                                                                             \
    friend class cereal::access;                                                                                                                                                                    \
    friend struct bungeegum::access;                                                                                                                                                                \
    template <typename value_t>                                                                                                                                                                     \
    friend struct bungeegum::detail::value_wrapper;                                                                                                                                                 \
    friend struct bungeegum::detail::widgets_manager;                                                                                                                                               \
    HSCPP_TRACK(classname, #classname)                                                                                                                                                              \
    std::uintptr_t _bungeegum_object_reference = 0;                                                                                                                                                 \
    hscpp_virtual void _bungeegum_load(cereal::JSONInputArchive& archive)                                                                                                                           \
    {                                                                                                                                                                                               \
        bungeegum::detail::serialize_fields<cereal::JSONInputArchive>(archive, std::string("BUNGEEGUM_OBJECT_REFERENCE, ") + std::string(#__VA_ARGS__), _bungeegum_object_reference, __VA_ARGS__);  \
    }                                                                                                                                                                                               \
    hscpp_virtual void _bungeegum_save(cereal::JSONOutputArchive& archive)                                                                                                                          \
    {                                                                                                                                                                                               \
        bungeegum::detail::serialize_fields<cereal::JSONOutputArchive>(archive, std::string("BUNGEEGUM_OBJECT_REFERENCE, ") + std::string(#__VA_ARGS__), _bungeegum_object_reference, __VA_ARGS__); \
    }

/// @brief
/// @details
#define HOTRELOAD_METHOD hscpp_virtual

namespace bungeegum {
namespace detail {

    /// @brief
    /// @tparam archive_t
    /// @tparam ...fields_t
    template <typename archive_t, typename... fields_t>
    void serialize_fields(archive_t& archive, const std::string& names, fields_t&&... fields);

    /// @brief
    /// @tparam value_t
    template <typename value_t>
    struct reloaded {
        reloaded() = delete;
        reloaded(const reloaded& other);
        reloaded& operator=(const reloaded& other);
        reloaded(reloaded&& other);
        reloaded& operator=(reloaded&& other);

        /// @brief
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
        reloader();
        reloader(const reloader& other) = delete;
        reloader& operator=(const reloader& other) = delete;
        reloader(reloader&& other) = default;
        reloader& operator=(reloader&& other) = default;

        /// @brief Allocates a new object that can be hotswapped, taking no argument.
        /// @exception Throws a compile-time exception if the widget type is not default-
        /// constructible.
        template <typename value_t>
        reloaded<value_t> allocate();

        /// @brief Gets the amount of memory blocks currently allocated.
        std::size_t allocated_blocks_count();

        /// @brief
        void add_define(const std::string& define);

        /// @brief Emplaces an include directory for recompilation.
        void add_include_directory(const std::filesystem::path& directory);

        /// @brief Emplaces a static library for recompilation.
        void add_library(const std::filesystem::path& file);

        /// @brief Emplaces a source directory for recompilation.
        void add_source_directory(const std::filesystem::path& directory);

        /// @brief Emplaces a source file that will be recompiled each time.
        void add_force_compiled_source_file(const std::filesystem::path& file);

        /// @brief Clears all files and directories provided for recompilation.
        void clear();

        /// @brief Forces an update, blocking the thread until recompilation has finished.
        void force_update();

        /// @brief Asynchronously waits for changes in the provided files and folders, triggers
        /// recompilation when needed and hotswaps allocated objects.
        reload_state update();

    private:
        std::shared_ptr<hscpp::Hotswapper> _swapper = nullptr;
        std::shared_ptr<hscpp::mem::UniqueRef<hscpp::mem::MemoryManager>> _manager = nullptr;
    };

    /// @brief
    struct reloaded_loader {
        reloaded_loader() = delete;
        reloaded_loader(const reloaded_loader& other) = delete;
        reloaded_loader& operator=(const reloaded_loader& other) = delete;
        reloaded_loader(reloaded_loader&& other) = default;
        reloaded_loader& operator=(reloaded_loader&& other) = default;

        /// @brief
        reloaded_loader(const std::filesystem::path& archive_path);

        /// @brief
        /// @tparam reloaded_value_t
        template <typename reloaded_value_t>
        void load(reloaded_value_t& value);

    private:
        std::ifstream _fstream;
        cereal::JSONInputArchive _archive;
    };

    /// @brief
    struct reloaded_saver {
        reloaded_saver() = delete;
        reloaded_saver(const reloaded_saver& other) = delete;
        reloaded_saver& operator=(const reloaded_saver& other) = delete;
        reloaded_saver(reloaded_saver&& other) = default;
        reloaded_saver& operator=(reloaded_saver&& other) = default;

        /// @brief
        reloaded_saver(const std::filesystem::path& archive_path);

        /// @brief
        /// @tparam reloaded_value_t
        template <typename reloaded_value_t>
        void save(reloaded_value_t& value);

    private:
        std::ofstream _fstream;
        cereal::JSONOutputArchive _archive;
    };
}
}

#else

/// @brief
/// @details
#define HOTRELOAD_CLASS(classname, ...)

/// @brief
/// @details
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
        using type = value_t&;
    };

    template <typename value_t>
    struct reference_type<value_t, std::enable_if_t<traits::is_reloadable_v<value_t>>> {
        using type = reloaded<value_t>;
    };

    /// @brief
    /// @tparam value_t
    template <typename value_t>
    using value_type_t = typename value_type<value_t>::type;

    /// @brief
    /// @tparam value_t
    template <typename value_t>
    using reference_type_t = typename reference_type<value_t>::type;
}
}

#include <bungeegum/glue/reload.inl>