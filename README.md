bungeegum
=========
	
![licence badge](https://img.shields.io/badge/licence-MIT-blue?style=for-the-badge)
![cpp badge](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/adriensalon/bungeegum/scheduled_release_windows.yml?label=windows&style=for-the-badge)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/adriensalon/bungeegum/scheduled_release_macos.yml?label=macos&style=for-the-badge)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/adriensalon/bungeegum/scheduled_release_ubuntu.yml?label=ubuntu&style=for-the-badge)
	
_bungeegum_ is a cross-platform C++17 library that offers a minimal and modular approach to implementing GUI widgets. It ships with a collection of widgets that mimic those of Flutter. Events and animations help facilitate synchronous and asynchronous operations that modify the GUI state and update changes on the screen. Additionally, a more advanced interface enables developers to augment widgets with lower-level functionalities. 

When a user interacts with the GUI, events are dispatched to the relevant widgets. These widgets can then react to the events and/or pass them on to their children. Widgets that require updating can modify their layout, following [Flutter's BoxLayout model](https://docs.flutter.dev/development/ui/layout/constraints), where widgets provide minimum and maximum size constraints to their children, who return the size they've chosen to their parent, which then positions them accordingly. Widgets that require redrawing can make changes to their draw command by submitting primitives, images, text, and other elements. 

In a effort to adhere to the [zero-overhead principle](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle), we only redraw the necessary areas of the screen. This library also optimizes the storage and iteration of widgets in an ECS tree container as much as possible. All user code invocations are resolved at compile-time using static polymorphism. Performance can be measured using a profiler integrated into an overlay, along with other tools available in debug builds.

## Motivation

When it comes to implementing a cross-platform GUI, I often struggle to determine the best tool for the job. For one, there are so many options available. Some tools are built on top of the operating system's pre-existing widgets, while others use GPU Graphics APIs to create their own widget systems, offering developers more granular control over the interface design.

High-level frameworks enable developers to create prototypes quickly and offer a sense of ease, as they provide a predefined style of programming that avoids the need to spend time refactoring code. However, these frameworks are often proprietary and require a level of expertise, as they incorporate specific concepts that developers must learn in order to create basic interfaces. While these tools can greatly enhance the productivity of large teams, as a solo developer, I have often regretted using them. After learning the basics, I found myself having to spend a lot of time implementing certain features because they required a much greater understanding of the framework, modification of it, or obscure undocumented configuration. In my experience, the most challenging aspect of using these frameworks is implementing features that were not necessarily anticipated by their developers.

Moving to lower level solutions may seem like a good idea, as they don't require as much specific knowledge to get started. However, using graphics APIs or wrappers to them can be a daunting task when building anything beyond very simple interfaces. [ImGui](https://github.com/imgui) attempts to bridge this gap by providing both low-level APIs for 2D rendering of text, images, and primitives, as well as higher-level APIs that implement many widgets, styles, docking, and other features. However, this approach of doing widgets can be difficult to extend. While it is simple and fast to create a basic GUI that works, achieving truly custom and polished GUIs can take much more time.

Not easy to choose when you don't have much and want your GUI to stand out. Then I discovered Flutter. It is heavy, it uses a managed langage that I don't like, but maybe it has the best vision about what a widget should be. They can be combined to create complex GUIs. They are highly customizable, and can be easily modified using named parameters to meet specific design needs. As i was frustrated with Dart, I wanted to try implementing this declarative syntax with generics, static polymorphism, `std::future`, etc...

## Quickstart

_bungeegum_ requires a C++17 compiler. CMake is used to build the library, gtest targets can be enabled with the `BUNGEEGUM_BUILD_TEST` option. A doxygen documentation target can be enabled with the `BUNGEEGUM_BUILD_DOC` option. All the widgets that ship with _bungeegum_ are implemented inside the `bungeegum::widgets` namespace.

See widgets [here](https://github.com/adriensalon/bungeegum/tree/master/bungeegum_widgets/README.md)

```C++
using namespace bungeegum::widgets;
```

We can either own the widgets or let _bungeegum_ own them and get references. With the [method chaining idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Parameter) and the factory function `bungeegum::make` we can defer the moment widgets are customized to after they are created, so that we can both create and modify them with the same syntax.

```C++
// we own the widget
Container my_container_widget;

// bungeegum owns the widget and we get a reference
Container& my_container_widget_ref = bungeegum::make<Container>();

auto& my_widget_tree = bungeegum::make<Center>()
    .child(bungeegum::make<Row>()			
        .height(200.f)
        .children(my_container_widget
            .width(120.f)
            .color({ 0.2f, 0.2f, 0.2f, 1.f }),
        my_container_widget_ref
            .width(140.f)
            .color({ 0.3f, 0.3f, 0.3f, 1.f })
        )
    );
```


### Create a window and a renderer

This library comes with everything needed to create a native window and a hardware accelerated renderer using [SDL2]() and [Diligent Engine]() on the most common platforms, such as Windows, MacOS, Linux, iOS, Android and Emscripten. If you want to use this library directly without binding to an existing game/engine, the `bungeegum::launch` function takes our widget tree as input, starts a window, a renderer, and blocks the thread until the window is closed. 

```C++
bungeegum::launch(my_widget_tree);
```

### ImGui as a backend

You may otherwise want to use this library to design GUIs along with an existing game/engine. As we use [ImGui](https://github.com/ocornut/imgui) as a backend for rendering geometry and collecting input events, we can initialize _bungeegum_ before starting a custom game loop with the `bungeegum::embed` function after ImGui has created a context. 

```C++
ImGui::CreateContext();
auto my_update_callback = bungeegum::embed(my_widget_tree);
```

It takes our widget tree as input and returns a callback we can call inside an ImGui render loop.

```C++
ImGui::NewFrame();
my_update_callback();
ImGui::Render();
```

## Limitations

### Images, fonts, shaders, transform, stencil clipping and assets

### Multithreading and optimization

## Flutter widgets

Lists all widgets specified here : https://docs.flutter.dev/ui/widgets/
Few widgets can't be implemented as for now and don't appear in this list

__Accessibility__
- 📕 ExcludeSemantics  
- 📕 MergeSemantics  
- 📕 Semantics  

__Animation and motion__
- 📕 AnimatedAlign
- 📕 AnimatededBuilder
- 📕 AnimatedContainer
- 📕 AnimatedCrossFade
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

__Assets, Images and Icons__
- 📕 AssetBundle ?
- 📕 Icon
- 📕 Image
- 📕 RawImage

__Async__
- 📗 FutureBuilder
- 📗 StreamBuilder

__Basics__
- 📙 Container
- 📕 MediaQuery
- 📕 Placeholder

__Input__
- 📕 Form
- 📕 FormField
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
- 📕 CustomPainter
- 📕 DecoratedBox
- 📕 FractionalTranslation
- 📕 Opacity
- 📕 RotatedBox
- 📕 Transform

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

## Implementing widgets

No macro or inheritance is required to implement widget classes. The `bungeegum::adopt` function registers widgets as children of the current widget. It can be called at any time, but usually widgets call it within their `child` or `children` methods to mimic the syntax of Flutter. We can use it inside our constructor to implement a basic widget with composition.

The `bungeegum::abandon` function unregisters widgets from being children of the current widget. All the widgets must have been adopted by this widget first. We are not required to call it before this widget gets destroyed.

```C++
struct my_widget_class {

	my_widget_class()
	{		
		bungeegum::adopt(this, bungeegum::make<Center>()
			.child(bungeegum::make<Row>()		
				.height(200.f)
				.children(_my_container_widget
						.width(120.f)
						.color({ 0.2f, 0.2f, 0.2f, 1.f }),
					bungeegum::make<Container>()
						.width(140.f)
						.color({ 0.3f, 0.3f, 0.3f, 1.f })
				)
			)
		);
	}
	
private:
	container _my_container_widget;
};
```

### Events

Event object are meant to wrap the boilerplate needed to store typed callbacks, pass them around and invoke them. Widgets can pass events to their children so that they can listen to / notify something going. We can either own events or let _bungeegum_ own them and get references.

```C++
// we own the event
bungeegum::event<float, float, std::string> my_event;

// bungeegum owns the event and we get a reference
auto& my_event_ref = bungeegum::make_event<float, float, std::string>();
```

The `bungeegum::event::on_trigger` method

```C++
my_event.on_trigger([] (const float& a, const float& b, const std::string& c) {
	std::cout << a << b << c << std::endl;
});
```

The `bungeegum::event::trigger` method

```
my_event.trigger(11.f, 22.f, "my string");
```
```
my_event.trigger(std::async([](){
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	return std::make_tuple({ 11.f, 22.f, "my string" }); // callbacks are triggered here
}));
```


The `bungeegum::event::merge` method

```C++
bungeegum::event my_event_1([] () { });
bungeegum::event my_event_2([] () { }).merge(my_event_1);
```


__Example__ : changing the width of a child widget when an asynchronous operation completes :

```C++
struct my_widget_class {

	my_widget_class() {
		bungeegum::adopt(this, bungeegum::make<center>()
			.child(bungeegum::make<row>()
				.children(_my_container
					.width(120.f) // initial width
					.color({ 0.2f, 0.2f, 0.2f, 1.f }), 
				bungeegum::make<container>()
					.width(120.f)
					.color({ 0.2f, 0.2f, 0.2f, 1.f })
				)
			)			
		);		

		// when the event triggers we set a new width 
		_my_event.on_triggered([this] (const my_http_result_class& my_http_result) {
			_my_container.width(my_http_result.http_resolved_width); 
		});
		
		// we want to trigger the event when an asynchonous operation completes
		my_event.trigger(std::async([this] () {
			my_http_result_class result = my_http_getter_function();
			return result;
		}));
	}

private:
	bungeegum::event<my_http_result_class> _my_event;
	container _my_container;
};
```



### Animations

Animation objects are meant to wrap the boilerplate needed to store typed callbacks, pass them around and invoke them with .... Widgets can pass animations to their children so that they can listen to / start / stop / modify something going. We can either own animations or let _bungeegum_ own them and get references.

```C++
// we own the animation
bungeegum::animation<float> my_animation;

// bungeegum owns the animation and we get a reference
auto& my_animation_ref = bungeegum::make_animation<float>();
```

The `bungeegum::animation::shape` method
The `bungeegum::animation::duration` method
The `bungeegum::animation::min` method
The `bungeegum::animation::max` method


```C++
bungeegum::animation<float4> my_animation
	.shape(animation_preset::bounce_in)
	.duration(std::chrono::seconds(1.f))
	.min(0.f)
	.max(1.f)
```

The `animation::bungeegum::on_tick` method
The `animation::bungeegum::start` method
The `animation::bungeegum::stop` method
The `animation::bungeegum::reset` method


```C++
	.on_tick([] (const float4 animated_color) {
		std::cout << animated_color << std::endl;
	})
	.start();
```

__Example__ : changing the color of a child widget with an animation :

```C++
class my_widget_class {

public:
	my_widget_class() {
		bungeegum::adopt(this, bungeegum::make<center>()
			.child(bungeegum::make<row>()
				.children(_my_container
					.width(120.f)
					.color({ 0.2f, 0.2f, 0.2f, 1.f }), // initial color
				bungeegum::make<container>()
					.width(120.f)
					.color({ 0.2f, 0.2f, 0.2f, 1.f })
				)
			)			
		);

		// when the animation ticks we set a new color
		_my_animation
			.min(0.f)
			.max(1.f)
			.duration(std::chrono::seconds(1.f))
			.shape(animation_preset::bounce_in)
			.on_tick([this] (const float4 animated_color) {
				_my_container.color(animated_color);
			})
			.start();
	}

private:
	bungeegum::animation<float4> _my_animation;
	container _my_container;
};
```


## Advanced usage

### Advanced user input interaction

```C++
void interact(bungeegum::interact_command& command)
{
}
```

### Advanced layout resolution

```C++
void resolve(bungeegum::resolve_command& command)
{
}
```

### Advanced drawing


```C++
void draw(bungeegum::draw_command& command)
{
}
```



## External dependencies

- [backwardcpp]() [] is
- [diligent engine]() [] is
- [doxygen awesome]() [] is
- [entt](https://github.com/skypjack/entt) [___MIT licence___]
- [glm]() []
- [gtest]() []
- [imgui](https://github.com/ocornut/imgui) [___MIT licence___]
- [implot]() []
- [sdl]() []
- [tinyspline]() []
