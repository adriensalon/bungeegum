#pragma once

#include <bungeegum/glue/reload.fwd>

namespace bungeegum {

/// @brief
/// @return
std::list<std::filesystem::path>& hotreload_include_directories();

/// @brief
/// @return
std::list<std::filesystem::path>& hotreload_source_directories();

}