if(BUNGEEGUM_BUILD_INSTALLER)
	project(bungeegum_installer)

	add_executable(bungeegum_installer "source/main.cpp")
	target_link_libraries(bungeegum_installer bungeegum_runtime)
	target_link_libraries(bungeegum_installer tiny-process-library)

	set_property(TARGET bungeegum_demo PROPERTY CXX_STANDARD 17)
endif()