add_subdirectory("farsitype")
add_subdirectory("glm")
add_subdirectory("imgui")
add_subdirectory("implot")
add_subdirectory("tinyspline")
add_subdirectory("cereal")
add_subdirectory("stb")

if(NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "Debug") # CMake defaults undefined build type to Debug
		add_subdirectory("backwardcpp")
	endif()

	if(NOT CMAKE_SYSTEM_NAME MATCHES "Android")
		set(NFD_INSTALL ON CACHE BOOL "" FORCE)
		add_subdirectory("nfdext")
		set_target_properties(nfd PROPERTIES LINKER_LANGUAGE CXX)
		target_include_directories(nfd PUBLIC "${CMAKE_CURRENT_LIST_DIR}/nfdext/src/include")

		add_subdirectory("hscpp")
		set(HSCPP_EXAMPLES OFF)
		set(HSCPP_TEST OFF)
		set(HSCPP_BUILD_EXTENSION_MEM ON)
	endif()
endif()

if(BUNGEEGUM_BUILD_TEST OR BUNGEEGUM_BUILD_PERF)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
	set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
	add_subdirectory("googletest")
endif()

if(BUNGEEGUM_BUILD_PERF)
	set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
	set(GOOGLETEST_PATH "${CMAKE_CURRENT_LIST_DIR}/googletest" CACHE PATH "" FORCE)
	add_subdirectory("benchmark")
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	add_subdirectory("sdl")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Android")
	set(DILIGENT_NO_DIRECT3D11 ON CACHE BOOL "" FORCE)
	set(DILIGENT_NO_DIRECT3D12 ON CACHE BOOL "" FORCE)
	set(DILIGENT_NO_METAL ON CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin" OR CMAKE_SYSTEM_NAME MATCHES "iOS")
	set(DILIGENT_NO_DIRECT3D11 ON CACHE BOOL "" FORCE)
	set(DILIGENT_NO_DIRECT3D12 ON CACHE BOOL "" FORCE)
	set(DILIGENT_NO_VULKAN ON CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(DILIGENT_NO_METAL ON CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(DILIGENT_NO_DIRECT3D11 ON CACHE BOOL "" FORCE)
	set(DILIGENT_NO_DIRECT3D12 ON CACHE BOOL "" FORCE)
	set(DILIGENT_NO_VULKAN ON CACHE BOOL "" FORCE)
	set(DILIGENT_NO_METAL ON CACHE BOOL "" FORCE)
else()
	message("\n-- Invalid platform : ${CMAKE_SYSTEM_NAME}")
	message("   Valid platform are [Windows, Darwin, Linux, Android, iOS, Emscripten]")
	return()
endif()

set(DILIGENT_NO_FORMAT_VALIDATION ON CACHE BOOL "" FORCE)
set(DILIGENT_NO_ARCHIVER ON CACHE BOOL "" FORCE)
add_subdirectory("diligentcore")


if(DEFINED BUNGEEGUM_INSTALL_DIR)
	file(
		COPY "${CMAKE_CURRENT_LIST_DIR}/cereal/include/cereal/"
		DESTINATION "${BUNGEEGUM_INSTALL_DIR}/runtime/include/cereal/")
	file(
		COPY "${CMAKE_CURRENT_LIST_DIR}/glm/glm/"
		DESTINATION "${BUNGEEGUM_INSTALL_DIR}/runtime/include/glm/")
	file(
		COPY "${CMAKE_CURRENT_LIST_DIR}/hscpp/include/hscpp/"
		DESTINATION "${BUNGEEGUM_INSTALL_DIR}/runtime/include/hscpp/")
	file(
		COPY "${CMAKE_CURRENT_LIST_DIR}/hscpp/extensions/mem/include/hscpp/mem/"
		DESTINATION "${BUNGEEGUM_INSTALL_DIR}/runtime/include/hscpp/mem/")
endif()
