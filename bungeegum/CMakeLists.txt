# _        _   _                 _
# | |      | | (_)               (_)
# ___| |_ __ _| |_ _  ___ __ _ _   _ _
# / __| __/ _` | __| |/ __/ _` | | | | |
# \__ \ || (_| | |_| | (_| (_| | |_| | |
# |___/\__\__,_|\__|_|\___\__, |\__,_|_|
# __/ |
# |___/

project(bungeegum)

set(bungeegum_source

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

add_library(bungeegum "${bungeegum_source}")
target_include_directories(bungeegum PUBLIC "include")
set_property(TARGET bungeegum PROPERTY CXX_STANDARD 17)

target_link_libraries(bungeegum glm)
target_link_libraries(bungeegum backward)
target_link_libraries(bungeegum imgui)
target_link_libraries(bungeegum implot)
target_link_libraries(bungeegum tinysplinecxx)
target_link_libraries(bungeegum SDL2-static)
target_link_libraries(bungeegum EnTT)
target_link_libraries(bungeegum vectorclass)
target_link_libraries(bungeegum Diligent-GraphicsEngine)
target_link_libraries(bungeegum Diligent-GraphicsTools)
target_link_libraries(bungeegum Diligent-ShaderTools)
target_link_libraries(bungeegum Diligent-Imgui)

target_include_directories(bungeegum PRIVATE "${CMAKE_SOURCE_DIR}/external/diligent/diligentcore/")
target_include_directories(bungeegum PRIVATE "${CMAKE_SOURCE_DIR}/external/diligent/diligenttools/")

target_link_libraries(bungeegum Diligent-GraphicsEngineD3D11-static)

# target_link_libraries(librender Diligent-GraphicsEngineD3D12-static)
# target_link_libraries(librender Diligent-GraphicsEngineOpenGL-static)
# target_link_libraries(librender Diligent-GraphicsEngineVulkan-static)