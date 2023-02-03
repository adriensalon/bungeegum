#       _        _   _                 _ 
#      | |      | | (_)               (_)
#   ___| |_ __ _| |_ _  ___ __ _ _   _ _ 
#  / __| __/ _` | __| |/ __/ _` | | | | |
#  \__ \ || (_| | |_| | (_| (_| | |_| | |
#  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
#                           __/ |        
#                          |___/         

project(staticgui_launch)

add_executable(staticgui_launch "main.cpp")
target_include_directories(staticgui_launch PRIVATE "staticgui/include")
target_link_libraries(staticgui_launch staticgui)

set_property(TARGET staticgui_launch PROPERTY CXX_STANDARD 17)