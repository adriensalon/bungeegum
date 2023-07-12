cmake_minimum_required(VERSION 3.0)
project(bungeegum_all)

option(BUNGEEGUM_BUILD_WIDGETS "Build widgets library" ON)
option(BUNGEEGUM_BUILD_DEMO "Build demo executable" ON)
option(BUNGEEGUM_BUILD_DOC "Build doxygen documentation" ON)
option(BUNGEEGUM_BUILD_TEST "Build gtest executable" OFF)

add_subdirectory("external")
add_subdirectory("bungeegum")
add_subdirectory("bungeegum_widgets")
add_subdirectory("demo")
add_subdirectory("doc")
add_subdirectory("test")
