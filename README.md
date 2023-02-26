# bungeegum

As energy consumption of devices is becoming an increasingly important consideration in application development, maybe we are moving towards porting GUIs to compiled languages...

_bungeegum_ is a cross-platform C++17 library that offers a minimal and modular approach to implementing GUI widgets. One way to use this library is by composing widgets from other widgets, as it ships with a collection of layouting widgets that mimic those of Flutter. Events and animations help facilitate synchronous and asynchronous operations that modify the GUI state and update changes on the screen. Additionally, _bungeegum_ provides a more advanced interface that enables developers to augment widgets with lower-level functionalities. When a user interacts with the GUI, events are dispatched to the relevant widgets. These widgets can then react to the events and/or pass them on to their children. Widgets that require updating can modify their layout, following [Flutter's BoxLayout model](https://docs.flutter.dev/development/ui/layout/constraints), where widgets provide minimum and maximum size constraints to their children, who return the size they've chosen to their parent, which then positions them accordingly. Widgets that require redrawing can make changes to their draw command by submitting primitives, images, text, and other elements. 

In a effort to adhere to the [zero-overhead principle](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle), we only redraw the necessary areas of the screen. This library also optimizes the storage and iteration of widgets in an ECS tree container as much as possible. All user code invocations are resolved at compile-time using static polymorphism. Performance can be measured using a profiler integrated into an overlay, along with other tools available in debug builds.

## Motivation

When it comes to implementing a cross-platform GUI, I often struggle to determine the best tool for the job. For one, there are so many options available. Some tools are built on top of the operating system's pre-existing widgets, while others use GPU Graphics APIs to create their own widget systems, offering developers more granular control over the interface design.

High-level frameworks enable developers to create prototypes quickly and offer a sense of ease, as they provide a predefined style of programming that avoids the need to spend time refactoring code. However, these frameworks are often proprietary and require a level of expertise, as they incorporate specific concepts that developers must learn in order to create basic interfaces. While these tools can greatly enhance the productivity of large teams, as a solo developer, I have often regretted using them. After learning the basics, I found myself having to spend a lot of time implementing certain features because they required a much greater understanding of the framework, modification of it, or obscure undocumented configuration. In my experience, the most challenging aspect of using these frameworks is implementing features that were not necessarily anticipated by their developers.

Moving to lower level solutions may seem like a good idea, as they don't require as much specific knowledge to get started. However, using graphics APIs or wrappers to them can be a daunting task when building anything beyond very simple interfaces. [ImGui](https://github.com/imgui) attempts to bridge this gap by providing both low-level APIs for 2D rendering of text, images, and primitives, as well as higher-level APIs that implement many widgets, styles, docking, and other features. However, this approach of doing widgets can be difficult to extend. While it is simple and fast to create a basic GUI that works, achieving truly custom and polished GUIs can take much more time.

Not easy to choose when you don't have much time and want your GUI to stand out. Then I discovered Flutter. It is heavy, it uses a managed langage that I don't like, but maybe it has the best syntax. One of its major advantages is its rich set of pre-built widgets, which can be combined to create complex GUIs. These widgets are highly customizable, and can be easily modified using named parameters to meet specific design needs. As i was frustrated with Dart, I wanted to try implementing this declarative syntax with generics, `std::future`, and so on.

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


> Mutable tree-based APIs suffer from a dichotomous access pattern: creating the tree’s original state typically uses a very different set of operations than subsequent updates.


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
