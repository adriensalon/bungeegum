#       _        _   _                 _ 
#      | |      | | (_)               (_)
#   ___| |_ __ _| |_ _  ___ __ _ _   _ _ 
#  / __| __/ _` | __| |/ __/ _` | | | | |
#  \__ \ || (_| | |_| | (_| (_| | |_| | |
#  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
#                           __/ |        
#                          |___/         

project(staticgui)

set(staticgui_source
	"source/utils/app.cpp"
	"source/utils/draw.cpp"
	"source/utils/ecs.cpp"
	"source/utils/id.cpp"

	"source/impl/context.cpp"
	"source/impl/event.cpp"
	"source/impl/layout.cpp"	
	"source/impl/tools.cpp"	
)

add_library(staticgui "${staticgui_source}")

target_include_directories(staticgui PUBLIC "include")

target_link_libraries(staticgui Diligent-GraphicsEngine)
target_link_libraries(staticgui Diligent-GraphicsTools)
target_link_libraries(staticgui Diligent-ShaderTools)
target_link_libraries(staticgui Diligent-Imgui)
target_link_libraries(staticgui imgui)
target_link_libraries(staticgui SDL2-static)
target_link_libraries(staticgui tree)
target_link_libraries(staticgui EnTT)

set_property(TARGET staticgui PROPERTY CXX_STANDARD 17)