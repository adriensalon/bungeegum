#pragma once

#include <bungeegum/glue/toolchain.hpp>

// directx is only available on Win32 and UWP platforms.
#define BUNGEEGUM_USE_DIRECTX (BUNGEEGUM_ENABLE_DIRECTX && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP))

// vulkan is available on any supported platform (with some prerequisite satisfied or not by CMake).
#define BUNGEEGUM_USE_VULKAN (BUNGEEGUM_ENABLE_VULKAN && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID))

// opengl is available on any supported platform.
#define BUNGEEGUM_USE_OPENGL BUNGEEGUM_ENABLE_OPENGL