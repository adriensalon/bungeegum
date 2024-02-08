#pragma once

#include <bungeegum/glue/toolchain.hpp>

#define BUNGEEGUM_USE_BACKTRACE (BUNGEEGUM_ENABLE_BACKTRACE && TOOLCHAIN_BUILD_DEBUG)

#if BUNGEEGUM_USE_BACKTRACE
#define BUNGEEGUM_USE_BACKTRACE_SIZE BUNGEEGUM_SET_BACKTRACE_SIZE
#else
#define BUNGEEGUM_USE_BACKTRACE_SIZE 0
#endif

// hotswap is only available on desktop platforms.
#define BUNGEEGUM_USE_HOTSWAP (BUNGEEGUM_ENABLE_HOTSWAP && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX))

// overlay is only available on desktop platforms.
#define BUNGEEGUM_USE_OVERLAY (BUNGEEGUM_ENABLE_OVERLAY && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_EMSCRIPTEN))