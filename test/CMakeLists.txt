if(BUNGEEGUM_BUILD_TEST)
	project(bungeegum_test)

	set(bungeegum_test_source

		"source/glue/test_constexpr.cpp"
		"source/glue/test_registry.cpp"
		"source/glue/test_toolchain.cpp"
		"source/glue/test_typelist.cpp"
		"source/test_bungeegum.cpp"
	)

	add_executable(bungeegum_test "${bungeegum_test_source}")
	target_link_libraries(bungeegum_test bungeegum)
	target_link_libraries(bungeegum_test gtest_main)
	add_test(NAME "bungeegum_test" COMMAND bungeegum_test)

	set_property(TARGET bungeegum_test PROPERTY CXX_STANDARD 17)

	if(DEFINED BUNGEEGUM_INSTALL_DIR)
		add_custom_command(
			TARGET bungeegum_test POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			$<TARGET_FILE_DIR:bungeegum_test>
			"${BUNGEEGUM_INSTALL_DIR}/test/bin/")
	endif()
endif()
