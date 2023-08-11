cmake_minimum_required(VERSION 3.0)

project(bungeegum_all)

set(BUNGEEGUM_VERSION "v0.1.0-alpha")

option(BUNGEEGUM_BUILD_BUNDLED_RUNTIME "Bundles external dependencies along with the runtime library" ON)
option(BUNGEEGUM_BUILD_WIDGETS "Build widgets library" ON)
option(BUNGEEGUM_BUILD_DEMO "Build demo executable" ON)
option(BUNGEEGUM_BUILD_DOC "Build doxygen documentation" ON)
option(BUNGEEGUM_BUILD_INSTALLER "Build installer executable" ON)
option(BUNGEEGUM_BUILD_TEST "Build gtest executable" ON)

option(BUNGEEGUM_ENABLE_BACKTRACE "Build stacktrace functionality to inspect the context where exceptions happen." ON)
option(BUNGEEGUM_ENABLE_HOTSWAP "Build selected user types so that they can be hotswapped on file save." ON)
option(BUNGEEGUM_ENABLE_OVERLAY "Build a GUI overlay above applications to facilitate development." ON)
option(BUNGEEGUM_ENABLE_EMBEDDED "Build embedded GUIs on all supported platforms (can not be used with Vulkan renderer)." ON)
option(BUNGEEGUM_ENABLE_STANDALONE "Build standalone applications on all supported platforms." ON)

message("[bungeegum] system name = ${CMAKE_SYSTEM_NAME}")
message("[bungeegum] runtime backtrace enabled = ${BUNGEEGUM_ENABLE_BACKTRACE}")
message("[bungeegum] runtime hotswap enabled = ${BUNGEEGUM_ENABLE_HOTSWAP}")
message("[bungeegum] runtime overlay enabled = ${BUNGEEGUM_ENABLE_OVERLAY}")
message("[bungeegum] embedded runtime enabled = ${BUNGEEGUM_ENABLE_EMBEDDED}")
message("[bungeegum] standalone runtime enabled = ${BUNGEEGUM_ENABLE_STANDALONE}")

add_subdirectory("external")
add_subdirectory("runtime")
add_subdirectory("widgets")
add_subdirectory("demo")
add_subdirectory("doc")
add_subdirectory("installer")
add_subdirectory("test")
