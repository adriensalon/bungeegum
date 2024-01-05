project(bungeegum_perf)

set(bungeegum_perf_source
	# "source/test_constexpr.cpp"
	# "source/test_registry.cpp"
	# "source/test_toolchain.cpp"
	# "source/test_typelist.cpp"
	"source/main.cpp"
)

add_executable(bungeegum_perf ${bungeegum_perf_source})

set_property(TARGET bungeegum_perf PROPERTY CXX_STANDARD 17)

target_link_libraries(bungeegum_perf bungeegum_runtime)
target_link_libraries(bungeegum_perf bungeegum_widgets)
target_link_libraries(bungeegum_perf benchmark)

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "--shell-file ${CMAKE_CURRENT_LIST_DIR}/platform/emscripten/shell.html") 
endif()
