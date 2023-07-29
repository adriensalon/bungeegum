if(BUNGEEGUM_BUILD_DEMO)
	project(bungeegum_demo)

	add_executable(bungeegum_demo "source/main.cpp")
	target_link_libraries(bungeegum_demo bungeegum_runtime)
	target_link_libraries(bungeegum_demo bungeegum_widgets)

	set_property(TARGET bungeegum_demo PROPERTY CXX_STANDARD 17)

	if(DEFINED BUNGEEGUM_INSTALL_DIR)
		add_custom_command(
			TARGET bungeegum_demo POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			$<TARGET_FILE_DIR:bungeegum_demo>
			"${BUNGEEGUM_INSTALL_DIR}/demo/bin/")
	endif()
endif()