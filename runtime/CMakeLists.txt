project(bungeegum_runtime)

if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Android")
	option(BUNGEEGUM_ENABLE_VULKAN "Build Vulkan renderer backend." ON)
	option(BUNGEEGUM_ENABLE_OPENGL "Build OpenGL renderer backend." ON)

elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin" OR CMAKE_SYSTEM_NAME MATCHES "iOS")
	option(BUNGEEGUM_ENABLE_OPENGL "Build OpenGL renderer backend." ON)
	option(BUNGEEGUM_ENABLE_METAL "Build Metal renderer backend." ON)

elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	option(BUNGEEGUM_ENABLE_DIRECTX "Build DirectX renderer backend." ON)
	option(BUNGEEGUM_ENABLE_VULKAN "Build Vulkan renderer backend." OFF)
	option(BUNGEEGUM_ENABLE_OPENGL "Build OpenGL renderer backend." ON)

elseif(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(BUNGEEGUM_ENABLE_OPENGL ON)

else()
	message("\n-- Invalid platform : ${CMAKE_SYSTEM_NAME}")
	message("   Valid platform are [Windows, Darwin, Linux, Android, iOS, Emscripten]")
	return()
endif()

set(bungeegum_runtime_source

	"source/glue/backtrace.cpp"
	"source/glue/console.cpp"
	"source/glue/dialog.cpp"
	"source/glue/hotswap.cpp"
	"source/glue/string.cpp"
	"source/glue/theme.cpp"
	"source/glue/time.cpp"
	"source/glue/rendering.cpp"
	"source/glue/rendering2.cpp"
	"source/glue/window.cpp"

	"source/core/font.cpp"
	"source/core/global.cpp"
	"source/core/log.cpp"
	"source/core/math.cpp"
	"source/core/pipeline.cpp"
	"source/core/shader.cpp"
	"source/core/texture.cpp"
	"source/core/widget.cpp"
	
	"source/overlay/build.cpp"
	"source/overlay/hierarchy.cpp"
	"source/overlay/gobuild.cpp"
	"source/overlay/menu.cpp"
	"source/overlay/output.cpp"
	"source/overlay/profiling.cpp"
	
	"source/static/inter_extrabold.cpp"
	"source/static/inter_regular.cpp"
)

add_library(bungeegum_runtime ${bungeegum_runtime_source})

target_include_directories(bungeegum_runtime PRIVATE "source/static/")
target_include_directories(bungeegum_runtime PUBLIC "include/")
target_include_directories(bungeegum_runtime PUBLIC "${CMAKE_SOURCE_DIR}/external/diligentcore/")

set_property(TARGET bungeegum_runtime PROPERTY CXX_STANDARD 17)

target_link_libraries(bungeegum_runtime glm)
target_link_libraries(bungeegum_runtime imgui)
target_link_libraries(bungeegum_runtime stb_image)
target_link_libraries(bungeegum_runtime tinysplinecxx)
target_link_libraries(bungeegum_runtime Diligent-BuildSettings)
target_link_libraries(bungeegum_runtime Diligent-Common)
target_link_libraries(bungeegum_runtime Diligent-PlatformInterface)
target_link_libraries(bungeegum_runtime Diligent-Primitives)
# target_link_libraries(bungeegum_runtime Diligent-GraphicsEngineInterface)
target_link_libraries(bungeegum_runtime Diligent-GraphicsEngine)
target_link_libraries(bungeegum_runtime Diligent-GraphicsTools)
target_link_libraries(bungeegum_runtime Diligent-ShaderTools)
target_link_libraries(bungeegum_runtime Diligent-TargetPlatform)

if(BUNGEEGUM_ENABLE_BACKTRACE AND NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten" AND CMAKE_BUILD_TYPE MATCHES "Debug")
	target_link_libraries(bungeegum_runtime backward)
endif()

if(BUNGEEGUM_ENABLE_OVERLAY)
	target_link_libraries(bungeegum_runtime implot)
endif()

if(BUNGEEGUM_ENABLE_OVERLAY OR BUNGEEGUM_ENABLE_HOTSWAP)
	target_link_libraries(bungeegum_runtime cereal)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	target_link_options(bungeegum_runtime PUBLIC "--std=c++17") 
	# target_link_options(bungeegum_runtime PUBLIC "-sMAX_WEBGL_VERSION=2") 
	# target_link_options(bungeegum_runtime PUBLIC "-sUSE_WEBGL2=1") 
	target_link_options(bungeegum_runtime PUBLIC "-sALLOW_MEMORY_GROWTH=1") 
	target_link_options(bungeegum_runtime PUBLIC "-sEXPORTED_RUNTIME_METHODS=['ccall','stackTrace','stringToNewUTF8']") 
	# -sGL_DEBUG=1
endif()

if(BUNGEEGUM_ENABLE_HOTSWAP AND BUNGEEGUM_ENABLE_OVERLAY AND NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	# target_link_libraries(bungeegum_runtime nfd)
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	target_link_libraries(bungeegum_runtime SDL2-static)
	if(BUNGEEGUM_ENABLE_HOTSWAP)
		target_link_libraries(bungeegum_runtime hscpp)
		target_link_libraries(bungeegum_runtime hscpp-mem)
	endif()
endif()

if(BUNGEEGUM_ENABLE_DIRECTX)
	target_link_libraries(bungeegum_runtime Diligent-GraphicsEngineD3D11-static)
	target_link_libraries(bungeegum_runtime Diligent-GraphicsEngineD3D12-static)
endif()

if(BUNGEEGUM_ENABLE_VULKAN)
	target_link_libraries(bungeegum_runtime Diligent-GraphicsEngineVk-static)
endif()

if(BUNGEEGUM_ENABLE_OPENGL)
	target_link_libraries(bungeegum_runtime Diligent-GraphicsEngineOpenGL-static)
endif()

if(BUNGEEGUM_ENABLE_METAL)
	target_link_libraries(bungeegum_runtime Diligent-GraphicsEngineMetal-static)
endif()

target_compile_definitions(bungeegum_runtime PUBLIC BUNGEEGUM_ENABLE_BACKTRACE)
target_compile_definitions(bungeegum_runtime PUBLIC BUNGEEGUM_ENABLE_HOTSWAP)
target_compile_definitions(bungeegum_runtime PUBLIC BUNGEEGUM_ENABLE_OVERLAY)
target_compile_definitions(bungeegum_runtime PUBLIC BUNGEEGUM_ENABLE_DIRECTX)
target_compile_definitions(bungeegum_runtime PUBLIC BUNGEEGUM_ENABLE_VULKAN)
target_compile_definitions(bungeegum_runtime PUBLIC BUNGEEGUM_ENABLE_OPENGL)

if(NOT EMSCRIPTEN)
	if(MSVC)
		target_compile_options(bungeegum_runtime PRIVATE /W4 /WX)
	else()
		# target_compile_options(bungeegum_runtime PRIVATE -Wall -Wextra -Wpedantic)
	endif()
endif()

# if(DEFINED BUNGEEGUM_INSTALL_DIR)
	# file(
	# 	COPY "${CMAKE_CURRENT_LIST_DIR}/include/"
	# 	DESTINATION "${BUNGEEGUM_INSTALL_DIR}/runtime/include/")

	list(APPEND static_libs bungeegum_runtime)

	function(_recursively_collect_dependencies input_target)
		set(_input_link_libraries LINK_LIBRARIES)
		get_target_property(_input_type ${input_target} TYPE)

		if(${_input_type} STREQUAL "INTERFACE_LIBRARY")
			set(_input_link_libraries INTERFACE_LINK_LIBRARIES)
		endif()

		get_target_property(public_dependencies ${input_target} ${_input_link_libraries})

		foreach(dependency IN LISTS public_dependencies)
			if(TARGET ${dependency})
				get_target_property(alias ${dependency} ALIASED_TARGET)

				if(TARGET ${alias})
					set(dependency ${alias})
				endif()

				get_target_property(_type ${dependency} TYPE)

				if(${_type} STREQUAL "STATIC_LIBRARY")
					list(APPEND static_libs ${dependency})
				endif()

				get_property(library_already_added
					GLOBAL PROPERTY _${tgt_name}_static_bundle_${dependency})

				if(NOT library_already_added)
					set_property(GLOBAL PROPERTY _${tgt_name}_static_bundle_${dependency} ON)
					_recursively_collect_dependencies(${dependency})
				endif()
			endif()
		endforeach()

		set(static_libs ${static_libs} PARENT_SCOPE)
	endfunction()

	_recursively_collect_dependencies(bungeegum_runtime)
	list(REMOVE_DUPLICATES static_libs)

	string(REPLACE ";" "," static_libs_str "${static_libs}")
	message(${static_libs_str})
	target_compile_definitions(bungeegum_runtime PUBLIC -DBUNGEEGUM_LIBS="${static_libs_str}")
	target_compile_definitions(bungeegum_runtime PUBLIC -DBUNGEEGUM_LIBS_PATH="${CMAKE_CURRENT_LIST_DIR}/../build/bin/")

	foreach(lib IN LISTS static_libs)
		# message(${lib})
		add_custom_command(
			TARGET bungeegum_runtime POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			$<TARGET_FILE_DIR:${lib}>
			"${CMAKE_CURRENT_LIST_DIR}/../build/bin/")
	endforeach()
# endif()
