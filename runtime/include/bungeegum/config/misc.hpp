#pragma once

// these will be set by CMake but they can be edited here for non CMake builds (?)

#if !defined(BUNGEEGUM_SET_TIME_UNIT)
#define BUNGEEGUM_SET_TIME_UNIT std::chrono::milliseconds
#endif

#if !defined(BUNGEEGUM_SET_INITIAL_VERTICES_SIZE)
#define BUNGEEGUM_SET_INITIAL_VERTICES_SIZE 1024u
#endif

#if !defined(BUNGEEGUM_SET_INITIAL_INDICES_SIZE)
#define BUNGEEGUM_SET_INITIAL_INDICES_SIZE 2048u
#endif

#include <bungeegum/config/misc.inl>