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
	add_subdirectory("diligent")
	add_subdirectory("sdl")
endif()

