cmake_minimum_required(VERSION 3.5)
project(farsitype)

add_library(farsitype "FarsiType.cpp")
target_include_directories(farsitype PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
set_property(TARGET farsitype PROPERTY CXX_STANDARD 17)
