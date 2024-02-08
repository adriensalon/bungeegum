#pragma once

#include <bungeegum/glue/toolchain.hpp>

// emscripten gives time as milliseconds
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#define BUNGEEGUM_USE_TIME_UNIT std::chrono::milliseconds
#else
#define BUNGEEGUM_USE_TIME_UNIT BUNGEEGUM_SET_TIME_UNIT
#endif
