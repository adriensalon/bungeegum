cmake_minimum_required(VERSION 3.0.0)
project(imgui)

set(imgui_source
	"imgui.cpp"
	"imgui_widgets.cpp"
	"imgui_tables.cpp"
	"imgui_draw.cpp"
	"imgui_demo.cpp"
	"backends/imgui_impl_sdl.cpp"
	"backends/imgui_impl_opengl3.cpp"

	"implot/implot.cpp"
	"implot/implot_items.cpp"
	"implot/implot_demo.cpp"

	"imnodes/imnodes.cpp"
)

# includes

# build
add_library(imgui ${imgui_source})
target_include_directories(imgui PRIVATE "../sdl/include/")
target_include_directories(imgui PRIVATE "../glad/include")
target_include_directories(imgui PUBLIC "backends/")
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

# link
target_link_libraries(imgui glad)

# properties
set_property(TARGET imgui PROPERTY CXX_STANDARD 17)
