# _
# | |__   _   _  _ __    __ _   ___   ___   __ _  _   _  _ __ ___
# | '_ \ | | | || '_ \  / _` | / _ \ / _ \ / _` || | | || '_ ` _ \
# | |_) || |_| || | | || (_| ||  __/|  __/| (_| || |_| || | | | | |
# |_.__/  \__,_||_| |_| \__, | \___| \___| \__, | \__,_||_| |_| |_|
# |___/              |___/

cmake_minimum_required(VERSION 3.0)
project(bungeegum_all)

option(BUNGEEGUM_BUILD_LAUNCHER "Build SDL + Diligent Engine launcher" ON)
option(BUNGEEGUM_BUILD_DOC "Build doxygen documentation" OFF)
option(BUNGEEGUM_BUILD_DEMO "Build demo executable" ON)
option(BUNGEEGUM_BUILD_TEST "Build gtest executable" OFF)

add_subdirectory("external")
add_subdirectory("bungeegum")
add_subdirectory("doc")
add_subdirectory("demo")
add_subdirectory("test")
