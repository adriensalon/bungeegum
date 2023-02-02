#       _        _   _                 _ 
#      | |      | | (_)               (_)
#   ___| |_ __ _| |_ _  ___ __ _ _   _ _ 
#  / __| __/ _` | __| |/ __/ _` | | | | |
#  \__ \ || (_| | |_| | (_| (_| | |_| | |
#  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
#                           __/ |        
#                          |___/         

set(DILIGENT_DEAR_IMGUI_PATH "${CMAKE_CURRENT_LIST_DIR}/../imgui/")
add_subdirectory("diligentcore")
add_subdirectory("diligenttools")

include_directories("diligentcore")