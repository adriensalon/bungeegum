cmake_minimum_required(VERSION 3.5)
project(stb_image)

add_library(stb_image "stb_image.cpp")

target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

set_property(TARGET stb_image PROPERTY CXX_STANDARD 17)