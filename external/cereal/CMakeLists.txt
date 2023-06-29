cmake_minimum_required(VERSION 3.0.0)
project(cereal)
add_library(cereal "source/cereal.cpp")

target_include_directories(cereal PUBLIC "include/")

set_property(TARGET cereal PROPERTY CXX_STANDARD 17)
