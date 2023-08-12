#pragma once

#include <bungeegum/backend/backend.fwd>

#if BUNGEEGUM_USE_HOTSWAP
namespace bungeegum {

/// @brief
std::vector<std::string>& defines();

/// @brief
std::vector<std::filesystem::path>& include_directories();

/// @brief
std::vector<std::filesystem::path>& libraries();

/// @brief
std::vector<std::filesystem::path>& source_directories();

/// @brief
std::vector<std::filesystem::path>& force_compiled_source_files();
}
#endif