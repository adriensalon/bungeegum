# libguiwidget

![licence badge](https://img.shields.io/badge/licence-MIT-blue?style=for-the-badge)
![cpp badge](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/adriensalon/bungeegum/scheduled_release_windows.yml?label=windows&style=for-the-badge)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/adriensalon/bungeegum/scheduled_release_macos.yml?label=macos&style=for-the-badge)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/adriensalon/bungeegum/scheduled_release_ubuntu.yml?label=ubuntu&style=for-the-badge)

## Motivation

### ___Use a well known and standard programming langage___

### ___Minimize unnecessary overhead___

### ___Bring Flutter widgets to any application___


## Syntax
## Features

### __Flutter widgets implemented in C++__


Lists all widgets specified here : https://docs.flutter.dev/ui/widgets/
Few widgets can't be implemented as for now and don't appear in this list

__Accessibility__
- 📕 ExcludeSemantics  
- 📕 MergeSemantics  
- 📕 Semantics  

__Animation and motion__
- 📙 AnimatedAlign
- 📙 AnimatededBuilder
- 📙 AnimatedContainer
- 📙 AnimatedCrossFade
- 📕 AnimatedDefaultTextStyle
- 📕 AnimatedList
- 📕 AnimatedModalBarrier
- 📕 AnimatedOpacity
- 📕 AnimatedPositioned
- 📕 AnimatedSize
- 📕 AnimatedWidget
- 📕 DecoratedBoxTransition
- 📕 FadeTransition
- 📕 Hero
- 📕 PositionedTransition
- 📕 RotationTransition
- 📕 ScaleTransition
- 📕 SizeTransition
- 📕 SlideTransition
- 📙 TweenAnimationBuilder

__Assets, Images and Icons__
- 📕 AssetBundle ?
- 📕 Icon
- 📙 Image
- 📕 RawImage

__Async__
- 📗 FutureBuilder
- 📗 StreamBuilder

__Basics__
- 📙 Container
- 📙 MediaQuery
- 📙 Placeholder
- 📙 SafeArea

__Input__
- 📙 Form
- 📙 FormField
- 📕 RawKeyboardListener

__Interaction model__
- 📙 AbsorbPointer
- 📙 Dismissible
- 📕 DragTarget
- 📕 Draggable
- 📙 GestureDetector
- 📕 IgnorePointer
- 📕 InteractiveViewer
- 📕 LongPressDraggable
- 📙 Listener
- 📙 MouseRegion

__Layout__ (Single child)
- 📙 Align
- 📙 AspectRatio
- 📕 Baseline ?
- 📙 Center
- 📗 ConstrainedBox
- 📗 CustomSingleChildLayout
- 📙 Expanded
- 📙 FittedBox
- 📕 FractionnalySizedBox
- 📕 IntrinsicHeight
- 📕 IntrinsicWidth
- 📗 LimitedBox
- 📕 Offstage
- 📗 OverflowBox
- 📗 Padding
- 📗 SizedBox
- 📕 SizedOverflowBox

__Layout__ (Multi child)
- 📕 Column
- 📙 CustomMultiChildLayout
- 📕 Flow
- 📕 GridView
- 📕 IndexedStack
- 📕 LayoutBuilder
- 📕 ListBody
- 📕 ListView
- 📕 Row
- 📕 Stack
- 📕 Table
- 📕 Wrap

__Layout__ (Sliver)
- 📕 CustomScrollView
- 📕 SliverChildBuilderDelegate
- 📕 SliverChildListDelegate
- 📕 SliverFixedExtentList
- 📕 SliverGrid
- 📕 SliverList
- 📕 SliverPadding
- 📕 SliverPersistentHeader
- 📕 SliverToBoxAdapter

__Painting__
- 📗 ColoredBox
- 📕 Canvas
- 📕 DecoratedBox
- 📕 FractionalTranslation
- 📙 Opacity
- 📕 RotatedBox
- 📙 Transform

__Scrolling__
- 📕 CustomScrollView
- 📕 DraggableScrollableSheet
- 📕 NestedScrollView
- 📕 NotificationListener
- 📕 ScrollConfiguration
- 📕 Scrollable
- 📕 SingleChildScrollView

__Text__
- 📕 DefaultTextStyle
- 📕 RichText
- 📕 Text

### __Hot reload__

### __Overlay__

### __Rendering misc__
Custom HLSL shaders


## Limitations

### __Multithreading__
### __Material and Cupertino widgets not implemented__
### __Hotreload specific limitations__

## Get started

### ___Build systems___

This library comes with CMake build files

```CMake
# Set the targets to configure
set(LIBGUIWIDGET_BUILD_DOC OFF)
set(LIBGUIWIDGET_BUILD_TEST OFF)
set(LIBGUIWIDGET_BUILD_PERF OFF)

# Set the options to enable
set(LIBGUIWIDGET_ENABLE_BACKTRACE ON)
set(LIBGUIWIDGET_ENABLE_HOTRELOAD ON)
set(LIBGUIWIDGET_ENABLE_OVERLAY ON)

# Add as subdirectory after setting wanted options
add_subdirectory("path to libguiwidget directory")

...

# Link libguiwidget to your library/executable
target_link_library(... libguiwidget)
```

### ___Compilers___

### __Create widgets__

Code for the runtime is located in ```libuiwidget.hpp``` and ???

```C++
#include <libuiwidget/libuiwidget.hpp>

using libuiwidget;
using libuiwidget::widgets;
```

Because of reasons explained here we can't use widgets allocated outside libuiwidget. We can create a widget or type ```T``` and get a reference to it with the ```make()``` free function.

```C++
T& my_widget = make<T>();
```

Taking a ```Container``` widget as an example we can leverage the method chaining idiom to set the properties we want after its creation.

```C++
Container& my_widget = make<Container>()
	.width(140.f)
	.color({ 0.3f, 0.3f, 0.3f, 1.f });
```

Best practice is to never use a raw pointer or reference to a widget after the end of the frame it was created because of the reasons explained here, and instead store a ```widget_ref<T>``` by creating your widget with the ```make_ref<T>()``` free function.

```C++
widget_ref<Container> my_typed_widget_ref = make_ref<Container>();
my_typed_widget_ref.get()
	.width(140.f)
	.color({ 0.3f, 0.3f, 0.3f, 1.f });
```

Direct references ```T&```, raw pointers ```T*```, and ```widget_ref<T>``` objects can be converted to type erased ```widget_id``` objects. This is useful to store widgets of unknown or different types. Most widgets implement their ```child(const widget_id&)``` or ```children(const std::vector<widget_id>&)``` like this.

Say smth about all the methods of widget_id and widget_ref 

```C++
widget_id my_type_erased_widget(my_typed_widget_ref);
widget_id my_other_type_erased_widget = make<Container>()
	.width(120.f)
	.color({ 0.2f, 0.2f, 0.2f, 1.f });
```

If we don't need to access widgets at all after we created them we can leverage the implicit conversion of ```T&``` or ```T*``` to ```widget_id```. Be careful, widgets must have been created using ```make<T>()``` or ```make_ref<T>()```.

```C++
widget_id my_widget_tree = make<Center>()
	.child(make<Row>()		
		.height(200.f)
		.children(make<Container>()
				.width(120.f)
				.color({ 0.2f, 0.2f, 0.2f, 1.f }),
			make<Container>()
				.width(140.f)
				.color({ 0.3f, 0.3f, 0.3f, 1.f })));		
```

Widgets can be destroyed with the ```destroy(widget_ref<T>& widget, const bool destroy_children)``` or the ```destroy(widget_id& widget, const bool destroy_children)``` free functions. After that a ```widget_ref<T>``` or a ```widget_id``` object will be in an undefined state and any call to its methods will trigger an exception.

### __Create a pipeline__

We create a window and a renderer with ```pipeline``` objects. 

```C++
pipeline my_pipeline;
my_pipeline.create_window();
my_pipeline.create_renderer<rendering_backend::opengl>();
my_pipeline.root(my_widget_tree);
```

And block the current thread.

```C++
my_pipeline.run(std::chrono::milliseconds(144));
```

Integration


```C++
while (true) {
	// do my rendering stuff
	my_pipeline.run_once();
}
```

### __Implement widgets with composition__


```C++
struct myWidget {

	myWidget& build()
	{
		adopt(this, make<Center>()
			.child(make<Row>()		
				.height(200.f)
				.children(make<Container>()
						.width(120.f)
						.color({ 0.2f, 0.2f, 0.2f, 1.f }),
					make<Container>()
						.width(140.f)
						.color({ 0.3f, 0.3f, 0.3f, 1.f }))));
		return *this;
	}
};
```

## Advanced

### __React to changes__


### __User input interaction__

```C++
void interact(bungeegum::interact_command& command)
{
}
```

### __Layout resolution__

```C++
void resolve(bungeegum::resolve_command& command)
{
}
```

### __Rendering__


```C++
void draw(bungeegum::draw_command& command)
{
}
```


## External dependencies

- [backwardcpp]() [] is
- [diligent engine]() [] is
- [doxygen awesome]() [] is
- [entt](https://github.com/skypjack/entt) MIT licence
- [glm]() []
- [gtest]() []
- [imgui](https://github.com/ocornut/imgui) MIT licence
- [implot]() []
- [sdl]() []
- [tinyspline]() []
