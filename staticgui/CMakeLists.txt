#       _        _   _                 _ 
#      | |      | | (_)               (_)
#   ___| |_ __ _| |_ _  ___ __ _ _   _ _ 
#  / __| __/ _` | __| |/ __/ _` | | | | |
#  \__ \ || (_| | |_| | (_| (_| | |_| | |
#  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
#                           __/ |        
#                          |___/         

project(staticgui)

set(staticgui_source

	"source/glue/backtrace.cpp"
	"source/glue/bspline.cpp"
	"source/glue/console.cpp"
	"source/glue/registry.cpp"
	"source/glue/renderer.cpp"
	"source/glue/simd.cpp"
	"source/glue/time.cpp"
	"source/glue/window.cpp"

	"source/state/animation.cpp"
	"source/state/context.cpp"
	"source/state/errors.cpp"
	"source/state/event.cpp"
	"source/state/layout.cpp"
	"source/state/rendering.cpp"
	"source/state/widget.cpp"

	"source/overlay/overlay.cpp"

	"source/staticgui.cpp"
)

add_library(staticgui "${staticgui_source}")
target_include_directories(staticgui PUBLIC "include")
set_property(TARGET staticgui PROPERTY CXX_STANDARD 17)

target_link_libraries(staticgui backward)
target_link_libraries(staticgui imgui)
target_link_libraries(staticgui implot)
target_link_libraries(staticgui tinysplinecxx)
target_link_libraries(staticgui SDL2-static)
target_link_libraries(staticgui EnTT)
target_link_libraries(staticgui vectorclass)
target_link_libraries(staticgui Diligent-GraphicsEngine)
target_link_libraries(staticgui Diligent-GraphicsTools)
target_link_libraries(staticgui Diligent-ShaderTools)
target_link_libraries(staticgui Diligent-Imgui)

target_include_directories(staticgui PRIVATE "${CMAKE_SOURCE_DIR}/external/diligent/diligentcore/")
target_include_directories(staticgui PRIVATE "${CMAKE_SOURCE_DIR}/external/diligent/diligenttools/")

target_link_libraries(staticgui Diligent-GraphicsEngineD3D11-static)
# target_link_libraries(librender Diligent-GraphicsEngineD3D12-static)
# target_link_libraries(librender Diligent-GraphicsEngineOpenGL-static)
# target_link_libraries(librender Diligent-GraphicsEngineVulkan-static)

