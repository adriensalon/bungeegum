#pragma once

// these will be set by CMake but they can be edited here for non CMake builds (?)

#if !defined(BUNGEEGUM_SET_TIME_UNIT)
#define BUNGEEGUM_SET_TIME_UNIT std::chrono::milliseconds
#endif

#include <bungeegum/config/misc.inl>