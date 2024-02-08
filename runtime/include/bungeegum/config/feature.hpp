#pragma once

// these will be set by CMake but they can be edited here for non CMake builds (?)

#if !defined(BUNGEEGUM_ENABLE_BACKTRACE)
/// @brief This enables backtracing for errors, warnings and messages emitted with the log_error(), 
/// log_warning() or log_message(). Traces containing function names, files and lines will be added
/// to the console output or to the log window of the overlay. Backtrace is only available on non 
/// Emscripten platforms for debug builds.
#define BUNGEEGUM_ENABLE_BACKTRACE 1
#endif

#if !defined(BUNGEEGUM_SET_BACKTRACE_SIZE)
/// @brief This represent the count of traces to get when emitting errors, warnings or messages
/// when backtrace has been enabled. Backtrace is only available on non Emscripten platforms for 
/// debug builds.
#define BUNGEEGUM_SET_BACKTRACE_SIZE 10
#endif

// hotswap

#if !defined(BUNGEEGUM_ENABLE_HOTSWAP)
#define BUNGEEGUM_ENABLE_HOTSWAP 1
#endif

// overlay

#if !defined(BUNGEEGUM_ENABLE_OVERLAY)
#define BUNGEEGUM_ENABLE_OVERLAY 1
#endif

#include <bungeegum/config/feature.inl>