if(BUNGEEGUM_BUILD_DOC)
	project(bungeegum_doc)

	find_package(Doxygen QUIET)

	if(DOXYGEN_FOUND)
		mark_as_advanced(DOXYGEN_EXECUTABLE)
	endif()

	file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/doc/doxygen/")

	add_custom_target(bungeegum_doc ALL
		COMMAND ${DOXYGEN_EXECUTABLE} "doc/Doxyfile"
		WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/.."
		VERBATIM
		USES_TERMINAL)
endif()
