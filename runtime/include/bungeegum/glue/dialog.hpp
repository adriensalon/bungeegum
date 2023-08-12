#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/toolchain.hpp>

/// Defines if we use desktop OS file picker dialogs.
#if !defined(BUNGEEGUM_USE_DIALOG)
#define BUNGEEGUM_USE_DIALOG (BUNGEEGUM_ENABLE_OVERLAY && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX))
#endif

#if BUNGEEGUM_USE_DIALOG

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct represent filters to use with desktop OS file picker APIs.
    struct dialog_extensions_filter {

        /// @brief Description for this filter, for example { "Libraries" }
        std::string description = {};

        /// @brief List of extensions, for example { "lib", "dll" }
        std::vector<std::string> extensions = {};
    };

    /// @brief Blocks the thread and gets a file path using desktop OS file picker APIs.
    std::optional<std::filesystem::path> open_file_dialog(
        const std::vector<dialog_extensions_filter>& filters = {},
        const std::optional<std::filesystem::path>& default_path = std::nullopt);

    /// @brief Blocks the thread and gets a file path for save using desktop OS file picker APIs.
    std::optional<std::filesystem::path> save_file_dialog(
        const std::vector<dialog_extensions_filter>& filters = {},
        const std::optional<std::filesystem::path>& default_path = std::nullopt,
        const std::optional<std::string>& default_name = std::nullopt);

    /// @brief Blocks the thread and gets a directory path using desktop OS file picker APIs.
    std::optional<std::filesystem::path> pick_folder_dialog(
        const std::optional<std::filesystem::path>& default_path = std::nullopt);
}
}

#endif