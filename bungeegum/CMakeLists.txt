project(bungeegum)

option(BUNGEEGUM_ENABLE_EMBEDDED "Embedded" ON)
option(BUNGEEGUM_ENABLE_STANDALONE "Build SDL + Diligent Engine launcher" ON)
option(BUNGEEGUM_ENABLE_OVERLAY "Enables an overlay diplaying errors etc" ON)

set(bungeegum_source

	"source/glue/backtrace.cpp"
	"source/glue/console.cpp"
	"source/glue/curve.cpp"
	"source/glue/imguarded.cpp"
	"source/glue/registry.cpp"
	"source/glue/reload.cpp"
	"source/glue/time.cpp"

	"source/core/animation.cpp"
	"source/core/event.cpp"
	"source/core/exceptions.cpp"

	"source/widget/context.cpp"
	"source/widget/draw.cpp"
	"source/widget/interact.cpp"
	"source/widget/resolve.cpp"
	"source/widget/widget.cpp"

	"source/backend/common.cpp"
	"source/backend/standalone.cpp"
	"source/backend/overlay/hierarchy.cpp"
	"source/backend/overlay/hotswapper.cpp"
	"source/backend/overlay/inspector.cpp"
	"source/backend/overlay/overlay.cpp"
	"source/backend/overlay/profiler.cpp"
	"source/backend/overlay/report.cpp"
	"source/backend/overlay/wireframe.cpp"
	"source/backend/overlay/compressed/fa4.cpp"
	"source/backend/overlay/compressed/inter_extrabold.cpp"
	"source/backend/overlay/compressed/inter_regular.cpp"
)

if(BUNGEEGUM_ENABLE_STANDALONE)
	set(bungeegum_source ${bungeegum_source}
		"source/glue/renderer.cpp"
		"source/glue/window.cpp")
endif()

add_library(bungeegum "${bungeegum_source}")
target_include_directories(bungeegum PUBLIC "include")
set_property(TARGET bungeegum PROPERTY CXX_STANDARD 17)

if(BUNGEEGUM_ENABLE_OVERLAY)
	target_compile_definitions(bungeegum PUBLIC BUNGEEGUM_ENABLE_OVERLAY)
endif()

target_compile_definitions(bungeegum PUBLIC BUNGEEGUM_BACKTRACE_SIZE=5)

if(MSVC)
	target_compile_options(bungeegum PRIVATE /W4 /WX)
else()
	target_compile_options(bungeegum PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(bungeegum glm)
target_link_libraries(bungeegum backward)
target_link_libraries(bungeegum cereal)
target_link_libraries(bungeegum imgui)
target_link_libraries(bungeegum implot)
target_link_libraries(bungeegum tinysplinecxx)
target_link_libraries(bungeegum EnTT)
target_link_libraries(bungeegum hscpp)
target_link_libraries(bungeegum hscpp-mem)

if(BUNGEEGUM_ENABLE_STANDALONE)
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