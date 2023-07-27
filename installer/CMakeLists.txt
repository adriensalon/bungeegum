if(BUNGEEGUM_BUILD_INSTALLER)
	project(bungeegum_installer)

	set(bungeegum_installer_source
		"source/glue/git.cpp"

		"source/tasks/build.cpp"
		"source/tasks/checkout.cpp"
		"source/tasks/configure.cpp"
	)

	add_executable(bungeegum_installer ${bungeegum_installer_source} "source/main.cpp")
	target_link_libraries(bungeegum_installer bungeegum_runtime)
	target_link_libraries(bungeegum_installer tiny-process-library)

	set_property(TARGET bungeegum_demo PROPERTY CXX_STANDARD 17)
endif()