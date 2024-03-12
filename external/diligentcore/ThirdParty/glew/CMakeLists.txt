cmake_minimum_required (VERSION 3.6)

project(glew-static)

set(SOURCE 
    src/glew.c
)

set(INCLUDE 
    include/GL/glew.h
    include/GL/wglew.h
)

add_library(glew-static STATIC ${SOURCE} ${INCLUDE})
set_common_target_properties(glew-static)

target_compile_definitions(glew-static 
PUBLIC
    GLEW_STATIC
    GLEW_NO_GLU
)

target_include_directories(glew-static 
PUBLIC
    include
)

target_link_libraries(glew-static PRIVATE Diligent-BuildSettings)
add_library(GLEW::glew ALIAS glew-static)

if(MSVC)
    target_compile_options(glew-static PRIVATE /wd4456)
endif()

source_group("src" FILES ${SOURCE})
source_group("include" FILES ${INCLUDE})

# MODIFIED
# target_compile_options(glew-static PRIVATE -Wno-reserved-macro-identifier)
# target_compile_options(glew-static PRIVATE -Wno-reserved-identifier)
# target_compile_options(glew-static PRIVATE -Wno-language-extension-token)
# target_compile_options(glew-static PRIVATE -Wno-strict-prototypes)
# target_compile_options(glew-static PRIVATE -Wno-nonportable-system-include-path)
# target_compile_options(glew-static PRIVATE -Wcast-function-type)
# ENDMODIFIED

set_target_properties(glew-static PROPERTIES
    FOLDER DiligentCore/ThirdParty
)
