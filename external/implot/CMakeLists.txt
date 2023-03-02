project(implot)

add_library(implot "implot.cpp" "implot_items.cpp" "implot_demo.cpp")
target_link_libraries(implot imgui)
target_include_directories(implot PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

set_property(TARGET implot PROPERTY CXX_STANDARD 17)