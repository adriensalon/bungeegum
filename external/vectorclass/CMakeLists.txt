cmake_minimum_required(VERSION 3.0.0)
project(vectorclass)

set(vectorclass_source "vectorclass.cpp")

add_library(vectorclass ${vectorclass_source})
target_include_directories(vectorclass PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

set_property(TARGET vectorclass PROPERTY CXX_STANDARD 17)
