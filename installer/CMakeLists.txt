if(BUNGEEGUM_BUILD_INSTALLER)
	project(bungeegum_installer)

	set(bungeegum_installer_source
		"source/glue/cmake.cpp"
		"source/glue/doxygen.cpp"
		"source/glue/git.cpp"
		"source/glue/tool.cpp"
	)

	add_executable(bungeegum_installer ${bungeegum_installer_source} "source/main.cpp")

	target_include_directories(bungeegum_installer PRIVATE "include/")

	set_property(TARGET bungeegum_demo PROPERTY CXX_STANDARD 17)

	target_link_libraries(bungeegum_installer PRIVATE bungeegum_runtime)
	target_link_libraries(bungeegum_installer PRIVATE tiny-process-library)

	target_compile_definitions(bungeegum_installer PRIVATE BUNGEEGUM_VERSION="${BUNGEEGUM_VERSION}")
endif()