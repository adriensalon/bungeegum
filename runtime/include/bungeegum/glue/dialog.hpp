#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/toolchain.hpp>

#if !defined(BUNGEEGUM_USE_DIALOG)
#define BUNGEEGUM_USE_DIALOG (BUNGEEGUM_ENABLE_OVERLAY && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX))
#endif

#if BUNGEEGUM_USE_DIALOG

namespace bungeegum {
namespace detail {

    /// @brief
    struct dialog_extensions_filter {
        std::string description = {};
        std::vector<std::string> extensions = {};
    };

    /// @brief
    std::optional<std::filesystem::path> open_file_dialog(const std::vector<dialog_extensions_filter>& filters);

    /// @brief
    std::optional<std::filesystem::path> save_file_dialog(const std::vector<dialog_extensions_filter>& filters);

}
}

#endif