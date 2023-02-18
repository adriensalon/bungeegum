# _        _   _                 _
# | |      | | (_)               (_)
# ___| |_ __ _| |_ _  ___ __ _ _   _ _
# / __| __/ _` | __| |/ __/ _` | | | | |
# \__ \ || (_| | |_| | (_| (_| | |_| | |
# |___/\__\__,_|\__|_|\___\__, |\__,_|_|
# __/ |
# |___/

project(bungeegum_test)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(bungeegum_test_source

	"source/glue/test_constexpr.cpp"
	"source/glue/test_registry.cpp"
	"source/glue/test_simd.cpp"
	"source/glue/test_toolchain.cpp"
	"source/glue/test_typelist.cpp"
	"source/test_bungeegum.cpp"
)

add_executable(bungeegum_test "${bungeegum_test_source}")
target_link_libraries(bungeegum_test bungeegum)
target_link_libraries(bungeegum_test gtest_main)
add_test(NAME "bungeegum_test" COMMAND bungeegum_test)

set_property(TARGET bungeegum_test PROPERTY CXX_STANDARD 17)
