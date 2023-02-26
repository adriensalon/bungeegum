# bungeegum

Although all guis share a lot of features, it's in the details that they can stand out.

Although the energy consumption of our devices is becoming an important issue for the development of applications, I do not feel that the trend is to port them to compiled languages.

bungeegum is a C++17 library to manage widgets and let them update themselves to achieve
- user input interaction inspired by browser events
- box layout inspired by flutter
- immediate mode painting inspired by imgui / skia


In a effort to adhere to the [zero-overhead principle](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle), this implementation only redraws the necessary areas of the screen. It also optimizes the storage and iteration of widgets in an ECS tree container as much as possible. All user code invocations are resolved at compile-time using static polymorphism. Performance can be measured using a profiler integrated into an overlay, along with other tools available in debug builds.


## Motivation

When it comes to implementing a graphical user interface (GUI), I often struggle to determine the best tool for the job. For one, there are so many options available. Some tools are built on top of the operating system's pre-existing widgets, while others use Graphics Processing Unit (GPU) Graphics APIs to create their own widget systems, offering developers more granular control over the interface design.

High-level frameworks enable developers to create prototypes quickly and offer a sense of ease, as they provide a predefined style of programming that avoids the need to spend time refactoring code. However, these frameworks are often proprietary and require a level of expertise, as they incorporate specific concepts that developers must learn in order to create basic interfaces. While these tools can greatly enhance the productivity of large teams, as a solo developer, I have often regretted using them. After learning the basics, I found myself unable to implement certain features because they required a greater understanding of the framework or extensive modification of it. In my experience, the most challenging aspect of using these frameworks is implementing features that were not necessarily anticipated by their developers.


Then come the lower level solutions. They don't require this initial amount of specific knowledge to achieve what we want. Building on top of graphics APIs or wrappers to them can be tempting when we already have a game that is using them. However, and unless the GUIs are really minimal it's going to be a huge effort to implement simple interfaces. [ImGui](https://github.com/ocornut/imgui) tries to close this gap.


Not easy to choose when you don't have much time and want your GUI to stand out. You must either demolish a castle to keep only the tower, or build it yourself in the middle of a field with cut stones. I guess I'm a bit of a maniac, but I feel a little sad when an application distributes Blink + V8 + Skia only to display a simple HTML page. Yes, the majority of devices will display this gui, but how much energy can we spend to save development time?

Then I discovered Flutter. It is heavy, it uses a managed langage that I don't like, 

Perhaps the best thing about the declarative syntax of [Flutter](https://flutter.dev/) is how we can instantiate widgets as trees, with each widget exposing the possibilities for customizing its behavior inside the tree with optional named parameters. This repository provides a basic implementation for a gui library that tries to mimick this syntax within C++17. However, we have to introduce several major architectural differences to Flutter in order to follow the [zero overhead principle](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle) and guarantee that :

 - no region of the screen will be drawn unless necessary
 - no logic code will run unless necessary
 - no widget data will be modified unless necessary

_Disclaimer : I don't hate Flutter_

 The Flutter framework makes extensive use of immutable data structures, memory preallocation and garbage collection to instantiate widget trees every time something has changed and some user code must react to it :
 
> Apps update their user interface in response to events (such as a user interaction) by telling the framework to replace a widget in the hierarchy with another widget. The framework then compares the new and old widgets, and efficiently updates the user interface. 

> Whenever the function is asked to build, the widget should return a new tree of widgets, regardless of what the widget previously returned. 

It becomes really problematic as the state data stored inside widgets grows in size, so Flutter solves this by separating widgets from their state :

> After being built, the widgets are held by the element tree, which retains the logical structure of the user interface. The element tree is necessary because the widgets themselves are immutable, which means (among other things), they cannot remember their parent or child relationships with other widgets. The element tree also holds the state objects associated with stateful widgets.

This means that even though they mitigated the "relationships issue" and the "heavy data structures issue" you still pay the price of deleting and allocating data structures, merging changes, binding to parent/children, and binding to an associated mutable state every time a widget's `build` method is called.

We cannot afford this approach that introduces some ugliness (users may want their widgets to store their data directly) and violates the "you don't pay for what you don't use" philosophy (you get huge overhead in the name of a specific coding syntax). What we want is a single tree of widgets, each containing its own data and relationships.

> Mutable tree-based APIs suffer from a dichotomous access pattern: creating the tree’s original state typically uses a very different set of operations than subsequent updates.

That is a fair point, we will get back to it.


## Build

- C++17 compiler is required
- [CMake](https://cmake.org/) build files are provided

### __Quickstart__

All the core widgets are implemented inside the `bungeegum::widgets` namespace.

```
using namespace bungeegum::widgets;
```

The user is not allowed to own the widgets. To be instantiated inside the bungeegum registry they must be constructed with the `bungeegum::make` function which forwards all the arguments to the constructor and returns a reference to the widget object. Unlike Dart, the C++ language does not allow the use of named parameters. However the [method chaining idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Parameter) provides us the parameter passing style we need :

```
auto& my_widget_tree = bungeegum::make<center>()
	.child(bungeegum::make<row>()			
		.height(200.f)
		.children(bungeegum::make<container>()
				.width(120.f)
				.color({ 0.2f, 0.2f, 0.2f, 1.f }),
			bungeegum::make<container>()
				.width(140.f)
				.color({ 0.3f, 0.3f, 0.3f, 1.f })
		)
	);
```

#### _Launch_

This library comes with everything needed to create a native window and a hardware accelerated renderer using [SDL2]() and [Diligent Engine]() on the most common platforms, such as Windows, MacOS, Linux, iOS, Android and Emscripten. If you want to use this library directly without binding to an existing game/engine, the `bungeegum::launch` function starts a window, a renderer, and blocks the thread until the window is closed. 

```
bungeegum::launch(my_widget_tree);
```

You can define a function that will be called after the renderer is initialized and before the update loop begins. This can be useful to setup custom state that requires the window or the renderer to be initialized.

```
bungeegum::launch(my_widget_tree, [&] () {
	// this code will be executed after renderer is initialized
	// but before update loop starts
});
```

This library behaves by 



#### _Embed_

You may want to use this library to design guis along with an existing game/engine. As we use [ImGui](https://github.com/ocornut/imgui) as a backend for rendering geometry and collecting input events, we can initialize bungeegum before starting a custom game loop with the `bungeegum::embed` function after ImGui has created a context. 

```
ImGui::CreateContext();
auto my_update_callback = bungeegum::embed(my_widget_tree);
```

It takes our widget tree as input and returns a callback we can call between our `ImGui::NewFrame` and `ImGui::Render` custom game/engine calls. 

```
ImGui::NewFrame();
my_update_callback();
ImGui::Render();
```

You may not care about the battery cost of swapping buffers each frame, especially if you are already implementing 3D rendering. However the bungeegum update callback given by `bungeegum::embed` will by default only render geometry where visual changes have to be redrawn. This is useful because we can bind a framebuffer, call our embed callback that may or may not clear or repaint it, and blit the framebuffer at the very end of the frame anyway.

```
ImGui::NewFrame();
my_bind_framebuffer_function(); // we use a framebuffer to store the gui
my_update_callback();
my_blit_framebuffer_to_screen_function();
ImGui::Render();
```

If you dont want to do this and instead force redrawing the whole gui every frame, just pass `true` to the embed callback.

```
ImGui::NewFrame();
my_update_callback(true); // instead we just force draw every frame
ImGui::Render();
```
### __Implementing widgets__

Must declare pas forcement on construction


> The `bungeegum::build` function will usually be called by each constructor. Widget inheritance is possible, and widgets can be declared more than once (explain here...).
You are really encouraged to take advantage of template deduction when implementing generic widgets
> Widgets can build template children by templating their constructors and accepting children as non-const references. Templating the widget struct would allow storing pointers or references to its children and having its members access them, but this is not always desirable. Depending on the use case this would prevent some useful usage of the `bungeegum::context::iterate` function. For example the widgets `my_widget_1<float>` and `my_widget_1<int>` would not be iterable at the same time.

```
my_widget::my_widget() {
	bungeegum::build(this, my_child_widget_type());
}
```

```
template <typename... children_widgets_t>
my_widget::my_widget(children_widgets_t&... my_other_children_widgets) {
	bungeegum::build(this, my_child_widget_type(), my_other_children_widgets...);
}
```




A basic widget looks like this :

```
struct my_widget_class {	

	my_widget_class() {
		auto& my_widget_tree = bungeegum::make<center>()
			.child(bungeegum::make<row>()			
				.height(200.f)
				.children(bungeegum::make<container>()
						.width(120.f)
						.color({ 0.2f, 0.2f, 0.2f, 1.f }),
					bungeegum::make<container>()
						.width(140.f)
						.color({ 0.3f, 0.3f, 0.3f, 1.f })
				)
			);
		bungeegum::adopt(this, my_widget_tree);
	}
};
```

So far so good. But what if we need to modify `my_widget_tree` at runtime ? First we need to be able to keep references to our children widgets after widget creation :

```
struct my_widget_class {	

	my_widget_class()
	{
		_my_center
			.child(_my_row		
				.height(200.f)
				.children(_my_container1
						.width(120.f)
						.color({ 0.2f, 0.2f, 0.2f, 1.f }),
					_my_container2
						.width(140.f)
						.color({ 0.3f, 0.3f, 0.3f, 1.f })
				)
			);
		bungeegum::adopt(this, _my_center_ref);
	}
	
private:
	center _my_center;
	row _my_row;
	container _my_container_1;
	container _my_container_2;
};
```

With the named parameter idiom widgets are mostly default constructed, delaying the "logical" object construction to setter methods. This means we are already using a set of operations intended to update objects in order to construct them. This is the exact opposite of what Flutter tries to achieve when implementing state modification with constructors. 



#### _Events_

To dynamically modify the gui state, event objects register callbacks of the same type and can be passed from a widget to its children to be triggered all at once when desired.

```
bungeegum::event<float, float, std::string> my_event;
```


```
my_event.on_trigger([] (const float& a, const float& b, const std::string& c) {
	std::cout << "" << std::endl;
});
```
```
my_event.trigger(11.f, 22.f, "my string");
```
```
my_event.trigger(std::async([](){
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	return std::make_tuple({ 11.f, 22.f, "my string" }); // callbacks are triggered here
}));
```

merge
```
bungeegum::event my_event_1([] () { });
bungeegum::event my_event_2([] () { }).merge(my_event_1);
```

attach/detach

```
void my_widget_class_method() {
	bungeegum::event my_event_1([] () { });

	// now the event will be destroyed when the update loop terminates
	my_event_1.detach(); 

	// now the event will be destroyed before this widget
	my_event_1.detach(this);

	// now the event will be destroyed when it exits scope (default behaviour !)
	my_event_1.attach();
}

```

Example : changing the width of a child widget when an asynchronous operation completes :

```
struct my_widget_class {

	my_widget_class() {

		// we use only composition
		bungeegum::adopt(bungeegum::make<center>()
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

		// when the event triggers we want to rebuild it with to a new width 
		_my_event.on_triggered([this] (const my_http_result_class& my_http_result) {
			_my_container.width(my_http_result.http_resolved_width); 
		}));
		
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



#### _Animations_

Example : changing the color of a child widget with an animation :

```
class my_widget_class {

public:
	my_widget_class() {
		using namespace bungeegum;

		// we create a specific child and keep a pointer to it
		_my_container_ptr = &(container()
			.width(120.f)
			.color({ 0.2f, 0.2f, 0.2f, 1.f }));

		// when the animation ticks we want to rebuild it with a new color
		_my_animation
			.min(0.f)
			.max(1.f)
			.duration(std::chrono::seconds(1.f))
			.shape(animation_preset::bounce_in)
			.on_tick([this] (const float4 animated_color) {
				_my_container_ptr->color(animated_color);
			})
			.start();

		// we build our child 
		build(
			center(
				row(
					my_container,
					container()
						.width(120.f)
						.color({ 0.2f, 0.2f, 0.2f, 1.f });
				)
			)
		);
	}

private:
	bungeegum::animation<float4> _my_animation;
	bungeegum::widgets::container_widget* _my_container_ptr = nullptr;
};
```


### __Advanced usage__

> In Flutter, everything is a widget. Widgets are just tiny chunks of UI that you can combine to make a complete app. Building an app Flutter is like building a lego set -- piece by piece. Widgets are nested inside of each other to build your app. Even the root of your app is just a widget. It's widgets all the way down. Flutter is unique in that every aspect of UI is handled with Widgets.

In Flutter even tricky/specific/low level concepts have already been encapsulated by widgets 

#### _Interact_

```
void on_interact(interact_command& command)
{
}
```

#### _Resolve_

```
template <typename... children_widgets_t>
float2 on_resolve(resolve_command& command, children_widgets_t&... children)
{
	// (command.constrain_child(children, command.constraint()), ...);
	std::cout << "resolving column !!! \n";
	return { 0.f, 0.f };
}
```

#### _Draw_


```
void on_draw(const float2 size, draw_command& command)
{
}
```

### __Limitations__

#### _Constructor encapsulation syntax_

No macro is required inside widget classes, but the user is not allowed to own them. They must be constructed with the `bungeegum::make` function which forwards all the arguments to the constructor. All the core widgets are implemented inside the `bungeegum::widgets` namespace.

```
auto& my_widget = bungeegum::make(bungeegum::widgets::center_widget(...));
```

As this requires a looot of typing, they use the __widget_ suffix and define an alias of `bungeegum::make<widget_t>` inside the `bungeegum` namespace. Then we can instead do this :

```
auto& my_widget = bungeegum::center(...);

```


if you reaaally need virtual inheritance among widget classes see the [CRTP idiom]()
#### _Images, fonts and shaders_

images fonts shaders etc


## Roadmap v0.0

Implementing some Flutter widgets behavior with bungeegum should be a good way to demonstrate and ensure that 
- [NOT DONE] align
- [NOT DONE] container
- [NOT DONE] column
- [NOT DONE] row

### __External dependencies__

- [bezier](https://github.com/oysteinmyrmo/bezier) [___MIT licence___] is
- [vectorclass](https://github.com/vectorclass/version2) [___Apache 2.0 licence___]
- [entt](https://github.com/skypjack/entt) [___MIT licence___]
- [imgui](https://github.com/ocornut/imgui) [___MIT licence___]
