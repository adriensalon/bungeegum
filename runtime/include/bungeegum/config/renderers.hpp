#pragma once

#include <bungeegum/glue/toolchain.fwd>

// these will be set by CMake but they can be edited here for non CMake builds (?)

#if !defined(LIBUIWIDGET_ENABLE_DIRECTX)
#define LIBUIWIDGET_ENABLE_DIRECTX 1
#endif

#if !defined(LIBUIWIDGET_ENABLE_VULKAN)
#define LIBUIWIDGET_ENABLE_VULKAN 0
#endif

#if !defined(LIBUIWIDGET_ENABLE_OPENGL)
#define LIBUIWIDGET_ENABLE_OPENGL 1
#endif

#include <bungeegum/config/renderers.inl>