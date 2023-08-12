add_subdirectory("glm")
add_subdirectory("imgui")
add_subdirectory("implot")
add_subdirectory("tinyprocess")
add_subdirectory("tinyspline")
add_subdirectory("entt")
add_subdirectory("backwardcpp")
add_subdirectory("cereal")

set(NFD_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory("nfdext")
set_target_properties(nfd PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory("hscpp")
set(HSCPP_EXAMPLES OFF)
set(HSCPP_TEST OFF)
set(HSCPP_BUILD_EXTENSION_MEM ON)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
add_subdirectory("googletest")

if(BUNGEEGUM_ENABLE_STANDALONE)
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

if(DEFINED BUNGEEGUM_INSTALL_DIR)
	file(
		COPY "${CMAKE_CURRENT_LIST_DIR}/cereal/include/cereal/"
		DESTINATION "${BUNGEEGUM_INSTALL_DIR}/runtime/include/cereal/")
	file(
		COPY "${CMAKE_CURRENT_LIST_DIR}/entt/src/entt/"
		DESTINATION "${BUNGEEGUM_INSTALL_DIR}/runtime/include/entt/")
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
