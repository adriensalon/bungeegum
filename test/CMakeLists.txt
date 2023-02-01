#       _        _   _                 _ 
#      | |      | | (_)               (_)
#   ___| |_ __ _| |_ _  ___ __ _ _   _ _ 
#  / __| __/ _` | __| |/ __/ _` | | | | |
#  \__ \ || (_| | |_| | (_| (_| | |_| | |
#  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
#                           __/ |        
#                          |___/         

project(staticgui_test)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(staticgui_test_source

	"source/glue/test_constexpr.cpp"
	"source/glue/test_registry.cpp"
	"source/test_staticgui.cpp"
)

add_executable(staticgui_test "${staticgui_test_source}")
target_link_libraries(staticgui_test staticgui)
target_link_libraries(staticgui_test gtest_main)
add_test(NAME "staticgui_test" COMMAND staticgui_test)

set_property(TARGET staticgui_test PROPERTY CXX_STANDARD 17)
