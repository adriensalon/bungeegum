#pragma once

// C++20 will grant us blessings

#include <bungeegum/glue/toolchain.hpp>

// Backends

// DirectX is only available on Win32 and UWP platforms
#define BUNGEEGUM_USE_DIRECTX (BUNGEEGUM_ENABLE_DIRECTX && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP))

// Vulkan is available on any supported platform (with some prerequisite satisfied or not by CMake)
#define BUNGEEGUM_USE_VULKAN (BUNGEEGUM_ENABLE_VULKAN && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID))

// OpenGL is available on any supported platform
#define BUNGEEGUM_USE_OPENGL BUNGEEGUM_ENABLE_OPENGL

// Features

/// @brief This enables backtracing for errors, warnings and messages emitted with the error(), 
/// warning() or message(). Traces containing function names, files and lines will be added
/// to the console output or to the log window of the overlay. Backtrace is only available on non 
/// Emscripten platforms for debug builds.
#define BUNGEEGUM_USE_BACKTRACE (BUNGEEGUM_ENABLE_BACKTRACE && TOOLCHAIN_BUILD_DEBUG && (TOOLCHAIN_PLATFORM_DESKTOP || TOOLCHAIN_PLATFORM_EMSCRIPTEN))

// hotswap is only available on desktop platforms
#define BUNGEEGUM_USE_HOTSWAP (BUNGEEGUM_ENABLE_HOTSWAP && TOOLCHAIN_PLATFORM_DESKTOP)

// overlay is only available on desktop platforms (and Emscripten, with limited functionnality)
#define BUNGEEGUM_USE_OVERLAY (BUNGEEGUM_ENABLE_OVERLAY && (TOOLCHAIN_PLATFORM_DESKTOP || TOOLCHAIN_PLATFORM_EMSCRIPTEN))

// Misc

#if BUNGEEGUM_USE_BACKTRACE
/// @brief This represent the count of traces to get when emitting errors, warnings or messages
/// when backtrace has been enabled. Backtrace is only available on non Emscripten platforms for 
/// debug builds.
#define BUNGEEGUM_USE_BACKTRACE_SIZE BUNGEEGUM_SET_BACKTRACE_SIZE
#else
/// @brief This represent the count of traces to get when emitting errors, warnings or messages
/// when backtrace has been enabled. Backtrace is only available on non Emscripten platforms for 
/// debug builds.
#define BUNGEEGUM_USE_BACKTRACE_SIZE 0
#endif

// Emscripten gives time as milliseconds
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
/// @brief
#define BUNGEEGUM_USE_TIME_UNIT std::chrono::milliseconds
#else
/// @brief
#define BUNGEEGUM_USE_TIME_UNIT BUNGEEGUM_SET_TIME_UNIT
#endif

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN // mobile ?
/// @brief
#define BUNGEEGUM_USE_INITIAL_WINDOW_WIDTH 0u
/// @brief
#define BUNGEEGUM_USE_INITIAL_WINDOW_HEIGHT 0u
/// @brief
#define BUNGEEGUM_USE_INITIAL_WINDOW_TITLE 0u
#else
/// @brief
#define BUNGEEGUM_USE_INITIAL_WINDOW_WIDTH BUNGEEGUM_SET_INITIAL_WINDOW_WIDTH
/// @brief
#define BUNGEEGUM_USE_INITIAL_WINDOW_HEIGHT BUNGEEGUM_SET_INITIAL_WINDOW_HEIGHT
/// @brief
#define BUNGEEGUM_USE_INITIAL_WINDOW_TITLE BUNGEEGUM_SET_INITIAL_WINDOW_TITLE
#endif

/// @brief
#define BUNGEEGUM_USE_INITIAL_VERTICES_SIZE BUNGEEGUM_SET_INITIAL_VERTICES_SIZE

/// @brief
#define BUNGEEGUM_USE_INITIAL_INDICES_SIZE BUNGEEGUM_SET_INITIAL_INDICES_SIZE
