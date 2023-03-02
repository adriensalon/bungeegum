#       _        _   _                 _ 
#      | |      | | (_)               (_)
#   ___| |_ __ _| |_ _  ___ __ _ _   _ _ 
#  / __| __/ _` | __| |/ __/ _` | | | | |
#  \__ \ || (_| | |_| | (_| (_| | |_| | |
#  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
#                           __/ |        
#                          |___/         

add_subdirectory("glm")
add_subdirectory("imgui")
add_subdirectory("implot")
add_subdirectory("tinyspline")
add_subdirectory("entt")
add_subdirectory("backwardcpp")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory("googletest")

if(BUNGEEGUM_BUILD_LAUNCHER)
	add_subdirectory("sdl")

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
	set(DILIGENT_NO_ARCHIVER ON CACHE BOOL "" FORCE) # implement later
	add_subdirectory("diligent")


	target_include_directories(Diligent-Imgui PUBLIC "${CMAKE_SOURCE_DIR}/external/diligent/diligentcore/")
	target_include_directories(Diligent-Imgui PUBLIC "${CMAKE_SOURCE_DIR}/external/diligent/diligenttools/")
endif()

