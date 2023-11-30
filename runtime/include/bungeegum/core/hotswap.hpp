#pragma once

#include <bungeegum/core/hotswap.fwd>

#if BUNGEEGUM_USE_HOTSWAP
namespace bungeegum {

/// @brief
namespace hotswap {

    /// @brief
    std::vector<std::string>& get_defines();

    /// @brief
    std::vector<std::filesystem::path>& get_include_directories();

    /// @brief
    std::vector<std::filesystem::path>& get_libraries();

    /// @brief
    std::vector<std::filesystem::path>& get_source_directories();

    /// @brief
    std::vector<std::filesystem::path>& get_force_compiled_source_files();
}
}
#endif