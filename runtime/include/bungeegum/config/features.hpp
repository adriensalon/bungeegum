#pragma once

#include <bungeegum/glue/toolchain.fwd>

// these will be set by CMake but they can be edited here for non CMake builds (?)

// With backtracing on, 
#if !defined(LIBUIWIDGET_ENABLE_BACKTRACE)
#define LIBUIWIDGET_ENABLE_BACKTRACE 1
#endif

#if !defined(LIBUIWIDGET_ENABLE_HOTSWAP)
#define LIBUIWIDGET_ENABLE_HOTSWAP 1
#endif

#if !defined(LIBUIWIDGET_ENABLE_MULTITHREADING)
#define LIBUIWIDGET_ENABLE_MULTITHREADING 0
#endif

#if !defined(LIBUIWIDGET_ENABLE_OVERLAY)
#define LIBUIWIDGET_ENABLE_OVERLAY 1
#endif

#include <bungeegum/config/features.inl>