if(BUNGEEGUM_BUILD_DEMO)
	project(bungeegum_demo)

	add_executable(bungeegum_demo "source/main.cpp")
	target_link_libraries(bungeegum_demo bungeegum)

	set_property(TARGET bungeegum_demo PROPERTY CXX_STANDARD 17)
endif()