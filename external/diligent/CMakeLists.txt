set(DILIGENT_DEAR_IMGUI_PATH "${CMAKE_CURRENT_LIST_DIR}/../imgui/")
add_subdirectory("diligentcore")
add_subdirectory("diligenttools")

include_directories("diligentcore")