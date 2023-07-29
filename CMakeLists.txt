cmake_minimum_required(VERSION 3.0)
project(bungeegum_all)

set(BUNGEEGUM_VERSION "v0.1.0-alpha")

option(BUNGEEGUM_BUILD_BUNDLED_RUNTIME "Bundles external dependencies along with the runtime library" ON)
option(BUNGEEGUM_BUILD_WIDGETS "Build widgets library" ON)
option(BUNGEEGUM_BUILD_DEMO "Build demo executable" ON)
option(BUNGEEGUM_BUILD_DOC "Build doxygen documentation" ON)
option(BUNGEEGUM_BUILD_INSTALLER "Build installer executable" ON)
option(BUNGEEGUM_BUILD_TEST "Build gtest executable" ON)

add_subdirectory("external")
add_subdirectory("runtime")
add_subdirectory("widgets")
add_subdirectory("demo")
add_subdirectory("doc")
add_subdirectory("installer")
add_subdirectory("test")
