project(bungeegum_widgets)

set(bungeegum_widgets_source

	"source/misc/Alignment.cpp"
	"source/misc/BoxConstraints.cpp"
	"source/misc/Color.cpp"
	"source/misc/EdgeInsets.cpp"
	"source/misc/Offset.cpp"
	"source/misc/Rect.cpp"
	"source/misc/Size.cpp"

	"source/widgets/Align.cpp"
	"source/widgets/ColoredBox.cpp"
	"source/widgets/ConstrainedBox.cpp"
	"source/widgets/Container.cpp"
	"source/widgets/Flex.cpp"
	"source/widgets/Padding.cpp"
	"source/widgets/Title.cpp"
)

add_library(bungeegum_widgets "${bungeegum_widgets_source}")
target_include_directories(bungeegum_widgets PUBLIC "include")
set_property(TARGET bungeegum_widgets PROPERTY CXX_STANDARD 17)

if(MSVC)
	target_compile_options(bungeegum_widgets PRIVATE /W4 /WX)
else()
	target_compile_options(bungeegum_widgets PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(bungeegum_widgets bungeegum)
