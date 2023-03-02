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
	"source/advanced/draw.cpp"
	"source/advanced/interact.cpp"
	"source/advanced/resolve.cpp"

	"source/core/animation.cpp"
	"source/core/context.cpp"
	"source/core/event.cpp"
	"source/core/exceptions.cpp"
	"source/core/widget.cpp"

	"source/glue/backtrace.cpp"
	"source/glue/console.cpp"
	"source/glue/curve.cpp"
	"source/glue/imguarded.cpp"
	"source/glue/registry.cpp"
	"source/glue/time.cpp"

	"source/overlay/compressed/fa4.cpp"
	"source/overlay/compressed/inter_extrabold.cpp"
	"source/overlay/compressed/inter_regular.cpp"
	"source/overlay/hierarchy.cpp"
	"source/overlay/inspector.cpp"
	"source/overlay/overlay.cpp"
	"source/overlay/profiler.cpp"
	"source/overlay/report.cpp"
	"source/overlay/wireframe.cpp")

if(BUNGEEGUM_BUILD_LAUNCHER)
	set(bungeegum_source ${bungeegum_source}
		"source/glue/renderer.cpp"
		"source/glue/window.cpp")
endif()

add_library(bungeegum "${bungeegum_source}")
target_include_directories(bungeegum PUBLIC "include")
set_property(TARGET bungeegum PROPERTY CXX_STANDARD 17)

target_link_libraries(bungeegum glm)
target_link_libraries(bungeegum backward)
target_link_libraries(bungeegum imgui)
target_link_libraries(bungeegum implot)
target_link_libraries(bungeegum tinysplinecxx)
target_link_libraries(bungeegum EnTT)

if(BUNGEEGUM_BUILD_LAUNCHER)
	target_link_libraries(bungeegum SDL2-static)
	target_link_libraries(bungeegum Diligent-Common)
	target_link_libraries(bungeegum Diligent-Primitives)
	target_link_libraries(bungeegum Diligent-GraphicsEngine)
	target_link_libraries(bungeegum Diligent-GraphicsTools)
	target_link_libraries(bungeegum Diligent-ShaderTools)
	target_link_libraries(bungeegum Diligent-Imgui)

	if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Android")
		target_link_libraries(bungeegum Diligent-GraphicsEngineOpenGL-static)
		target_link_libraries(bungeegum Diligent-GraphicsEngineVk-static)
	elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin" OR CMAKE_SYSTEM_NAME MATCHES "iOS")
		target_link_libraries(bungeegum Diligent-GraphicsEngineOpenGL-static)
		target_link_libraries(bungeegum Diligent-GraphicsEngineMetal-static)
	elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
		target_link_libraries(bungeegum Diligent-GraphicsEngineD3D11-static)
		target_link_libraries(bungeegum Diligent-GraphicsEngineD3D12-static)

	# target_link_libraries(bungeegum Diligent-GraphicsEngineVk-static)
	elseif(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
		target_link_libraries(bungeegum Diligent-GraphicsEngineOpenGL-static)
	else()
		message("\n-- Invalid platform : ${CMAKE_SYSTEM_NAME}")
		message("   Valid platform are [Windows, Darwin, Linux, Android, iOS, Emscripten]")
		return()
	endif()
endif()