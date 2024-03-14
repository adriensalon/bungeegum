#pragma once

#include <bungeegum/glue/toolchain.hpp>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct represent filters to use with desktop OS file picker
    struct dialog_extensions_filter {

        /// @brief Description for this filter, for example { "Libraries" }
        std::string description = {};

        /// @brief List of extensions, for example { "lib", "dll" }
        std::vector<std::string> extensions = {};
    };

#if TOOLCHAIN_PLATFORM_DESKTOP

    /// @brief Blocks the thread and gets a file path using desktop OS file picker
    /// @param filters
    /// @param default_path
    [[nodiscard]] std::optional<std::filesystem::path> open_file_dialog(
        const std::vector<dialog_extensions_filter>& filters = {},
        const std::optional<std::filesystem::path>& default_path = std::nullopt);

    /// @brief Blocks the thread and gets a file path for save using desktop OS file picker
    /// @param filters
    /// @param default_path
    /// @param default_name
    [[nodiscard]] std::optional<std::filesystem::path> save_file_dialog(
        const std::vector<dialog_extensions_filter>& filters = {},
        const std::optional<std::filesystem::path>& default_path = std::nullopt,
        const std::optional<std::string>& default_name = std::nullopt);

    /// @brief Blocks the thread and gets a directory path using desktop OS file picker
    /// @param default_path
    [[nodiscard]] std::optional<std::filesystem::path> pick_folder_dialog(
        const std::optional<std::filesystem::path>& default_path = std::nullopt);

#endif
}
}
