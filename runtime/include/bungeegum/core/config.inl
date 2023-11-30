#pragma once

// directx is only available on Win32 and UWP platforms.
#define BUNGEEGUM_USE_DIRECTX (BUNGEEGUM_ENABLE_DIRECTX && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP))

// vulkan is available on any supported platform (with some prerequisite satisfied or not by CMake).
#define BUNGEEGUM_USE_VULKAN BUNGEEGUM_ENABLE_VULKAN

// opengl is available on any supported platform.
#define BUNGEEGUM_USE_OPENGL BUNGEEGUM_ENABLE_OPENGL

// standalone is available on any supported platform.
#define BUNGEEGUM_USE_STANDALONE BUNGEEGUM_ENABLE_STANDALONE

// embedded is available on any supported platform.
#define BUNGEEGUM_USE_EMBEDDED BUNGEEGUM_ENABLE_EMBEDDED

// backtrace is only available on non Emscripten platforms for debug builds.
#define BUNGEEGUM_USE_BACKTRACE (BUNGEEGUM_ENABLE_BACKTRACE && TOOLCHAIN_BUILD_DEBUG && !TOOLCHAIN_PLATFORM_EMSCRIPTEN)

// hotswap is only available on desktop platforms.
#define BUNGEEGUM_USE_HOTSWAP (BUNGEEGUM_ENABLE_HOTSWAP && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX))

// overlay is only available on desktop platforms.
#define BUNGEEGUM_USE_OVERLAY (BUNGEEGUM_ENABLE_OVERLAY && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_EMSCRIPTEN))