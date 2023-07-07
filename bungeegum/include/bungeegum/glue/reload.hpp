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

#include <bungeegum/glue/foreach.hpp>
#include <bungeegum/glue/simd.hpp>

namespace hscpp {
class Hotswapper;
namespace mem {
    class MemoryManager;
}
}

#define BUNGEEGUM_STRINGIFY_IMPL(value) #value
#define BUNGEEGUM_STRINGIFY(value) BUNGEEGUM_STRINGIFY_IMPL(value)

#define BUNGEEGUM_CONCAT_IMPL(value_a, value_b) value_a##value_b
#define BUNGEEGUM_CONCAT(value_a, value_b) BUNGEEGUM_CONCAT_IMPL(value_a, value_b)

/// @brief
/// @details
#define HOTRELOAD_CLASS(classname, ...)                                                                                                                             \
    friend class cereal::access;                                                                                                                                    \
    template <typename value_t>                                                                                                                                     \
    friend struct bungeegum::detail::value_wrapper;                                                                                                                 \
    HSCPP_TRACK(classname, #classname)                                                                                                                              \
    std::uintptr_t _raw_object = 0;                                                                                                                                 \
    hscpp_virtual void _bungeegum_load(cereal::JSONInputArchive& archive)                                                                                           \
    {                                                                                                                                                               \
        bungeegum::detail::serialize_fields<cereal::JSONInputArchive>(archive, std::string("RAW_OBJECT, ") + std::string(#__VA_ARGS__), _raw_object, __VA_ARGS__);  \
    }                                                                                                                                                               \
    hscpp_virtual void _bungeegum_save(cereal::JSONOutputArchive& archive)                                                                                          \
    {                                                                                                                                                               \
        bungeegum::detail::serialize_fields<cereal::JSONOutputArchive>(archive, std::string("RAW_OBJECT, ") + std::string(#__VA_ARGS__), _raw_object, __VA_ARGS__); \
    }

/// @brief
/// @details
#define HOTRELOAD_METHOD hscpp_virtual

namespace bungeegum {
namespace detail {

    // trait is reloadable

    /// @brief
    /// @tparam archive_t
    /// @tparam ...fields_t
    template <typename archive_t, typename... fields_t>
    void serialize_fields(archive_t& archive, const std::string& names, fields_t&&... fields);

    template <typename value_t>
    struct reloaded {
        reloaded() = delete;
        reloaded(const reloaded& other);
        reloaded& operator=(const reloaded& other);
        reloaded(reloaded&& other);
        reloaded& operator=(reloaded&& other);

        value_t& get();
        const value_t& get() const;

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

#include <bungeegum/glue/reload.inl>

#else

#define HOTRELOAD_CLASS(class, name)
#define HOTRELOAD_METHOD
#define HOTRELOAD_FIELDS(...)

// namespace bungeegum {
// namespace detail {

//     template <typename value_t>
//     using reloaded = std::reference_wrapper<value_t>;
// }
// }

#endif