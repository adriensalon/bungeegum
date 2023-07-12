#pragma once

#include <bungeegum/backend/backend.fwd>

namespace bungeegum {

/// @brief
/// @return
std::list<std::string>& hotreload_defines();

/// @brief
/// @return
std::list<std::filesystem::path>& hotreload_include_directories();

/// @brief
/// @return
std::list<std::filesystem::path>& hotreload_libraries();

/// @brief
/// @return
std::list<std::filesystem::path>& hotreload_source_directories();

/// @brief
/// @return
std::list<std::filesystem::path>& hotreload_force_compiled_source_files();
}