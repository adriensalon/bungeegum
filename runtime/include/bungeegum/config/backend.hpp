#pragma once

// these will be set by CMake but they can be edited here for non CMake builds (?)

#if !defined(BUNGEEGUM_ENABLE_DIRECTX)
#define BUNGEEGUM_ENABLE_DIRECTX 1
#endif

#if !defined(BUNGEEGUM_ENABLE_VULKAN)
#define BUNGEEGUM_ENABLE_VULKAN 0
#endif

#if !defined(BUNGEEGUM_ENABLE_OPENGL)
#define BUNGEEGUM_ENABLE_OPENGL 1
#endif

#include <bungeegum/config/backend.inl>