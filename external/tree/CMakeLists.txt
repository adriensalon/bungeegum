project(tree)

add_library(tree "src/tree.cpp")
target_include_directories(tree PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")