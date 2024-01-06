#pragma once

#include <bungeegum/core/hotswap.fwd>

namespace bungeegum {

#if BUNGEEGUM_USE_HOTSWAP
/// @brief
[[nodiscard]] std::vector<std::string>& get_defines();

/// @brief
[[nodiscard]] std::vector<std::filesystem::path>& get_include_directories();

/// @brief
[[nodiscard]] std::vector<std::filesystem::path>& get_libraries();

/// @brief
[[nodiscard]] std::vector<std::filesystem::path>& get_source_directories();

/// @brief
[[nodiscard]] std::vector<std::filesystem::path>& get_force_compiled_source_files();
#endif

}