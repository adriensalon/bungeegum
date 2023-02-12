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

	"source/context/animation.cpp"
	"source/context/context.cpp"
	"source/context/draw.cpp"
	"source/context/event.cpp"
	"source/context/exceptions.cpp"
	"source/context/resolve.cpp"
	"source/context/widget.cpp"

	"source/glue/backtrace.cpp"
	"source/glue/console.cpp"
	"source/glue/curve.cpp"
	"source/glue/imguarded.cpp"
	"source/glue/registry.cpp"
	"source/glue/renderer.cpp"
	"source/glue/time.cpp"
	"source/glue/window.cpp"

	"source/overlay/compressed/fa4.cpp"
	"source/overlay/compressed/inter_extrabold.cpp"
	"source/overlay/compressed/inter_regular.cpp"
	"source/overlay/hierarchy.cpp"
	"source/overlay/inspector.cpp"
	"source/overlay/overlay.cpp"
	"source/overlay/profiler.cpp"
	"source/overlay/report.cpp"
	"source/overlay/wireframe.cpp"

)

add_library(staticgui "${staticgui_source}")
target_include_directories(staticgui PUBLIC "include")
set_property(TARGET staticgui PROPERTY CXX_STANDARD 17)

target_link_libraries(staticgui glm)
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

