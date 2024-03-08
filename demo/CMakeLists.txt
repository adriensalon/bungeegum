project(bungeegum_demo)

add_subdirectory("external")

set(bungeegum_demo_source
	"source/main.cpp"

	"static/myfont.cpp"
)

add_executable(bungeegum_demo ${bungeegum_demo_source})

target_include_directories(bungeegum_demo PRIVATE "static")

set_property(TARGET bungeegum_demo PROPERTY CXX_STANDARD 17)

target_link_libraries(bungeegum_demo bungeegum_runtime)
target_link_libraries(bungeegum_demo bungeegum_widgets)

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "--shell-file ${CMAKE_CURRENT_LIST_DIR}/../platform/Emscripten/singlepage.html") 
endif()