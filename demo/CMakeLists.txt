if(BUNGEEGUM_BUILD_DEMO)
	project(bungeegum_demo)

	add_executable(bungeegum_demo "source/main.cpp")
	target_link_libraries(bungeegum_demo bungeegum_runtime)
	target_link_libraries(bungeegum_demo bungeegum_widgets)

	set_property(TARGET bungeegum_demo PROPERTY CXX_STANDARD 17)

	
	if(EMSCRIPTEN)		
		set(CMAKE_EXECUTABLE_SUFFIX ".html")
		target_link_options(bungeegum_demo PRIVATE "SHELL: --shell-file '${CMAKE_CURRENT_LIST_DIR}/source/shell.html'") 
		# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3 --shell-file ${CMAKE_CURRENT_LIST_DIR}/source/shell.html --std=c++17 --preload-file ${CMAKE_CURRENT_LIST_DIR}/../install/compiled@/ --use-preload-cache") # -s USE_FREETYPE=1 -s DISABLE_EXCEPTION_CATCHING=0 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 --bind -s WASM=1 -O2 -s LEGACY_GL_EMULATION=0  -s GL_UNSAFE_OPTS=0 --pre-js pre-module.js --post-js post-module.js -s ASSERTIONS=1 -s GL_ASSERTIONS=1 -s INVOKE_RUN=0  -std=c++11 -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s OFFSCREENCANVAS_SUPPORT=1 --preload-file textures --preload-file shaders --preload-file fonts")
		# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--shell-file ${CMAKE_CURRENT_LIST_DIR}/source/shell.html") 
		# set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s LLD_REPORT_UNDEFINED -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s USE_WEBGL2=1 -s GL_DEBUG=1 -s FETCH")
		set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-O3 -s LLD_REPORT_UNDEFINED -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s USE_WEBGL2=1 -s GL_DEBUG=1 -s FETCH")
	endif()

endif()