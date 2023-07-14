#pragma once

#include <bungeegum/backend/backend.fwd>

#if BUNGEEGUM_USE_HOTSWAP
namespace bungeegum {

/// @brief
std::list<std::string>& defines();

/// @brief
std::list<std::filesystem::path>& include_directories();

/// @brief
std::list<std::filesystem::path>& libraries();

/// @brief
std::list<std::filesystem::path>& source_directories();

/// @brief
std::list<std::filesystem::path>& force_compiled_source_files();
}
#endif