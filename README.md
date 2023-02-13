# staticgui

This repository provides a basic implementation for a gui library that tries to mimick the declarative syntax of [flutter widgets](https://flutter.dev/) with C++17.


#### _Box layout implementation_

#### _Immediate mode rendering_

#### _No style policy is the best policy_

#### _Build only when needed_


 while taking advantage of the C++17 type system to achieve static polymorphism


 only needed elements when needed with gpu acceleration, with a declarative syntax similar to flutter widgets, but relying on templates to achieve static polymorphism. The doxygen-generated documentation can be found [here](https://okok.org/) or built along the library. 

 It comes with additional debugging tools on desktop platforms

### __Build__
Windows/MacOS/iOS/Linux/Android platform, but only tested on Windows so far

- a C++17 compiler is required
- [CMake](https://cmake.org/) build files are provided to facilitate the compilation


### __Quickstart__

Unlike Dart, the C++ language does not allow the use of named parameters. However the [method chaining idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Parameter)


```
auto& my_widget_tree = 
	staticgui::center(
		staticgui::row(
			staticgui::container()
				.width(120.f)
				.color({ 0.2f, 0.2f, 0.2f, 1.f }),
			staticgui::container()
				.width(140.f)
				.color({ 0.3f, 0.3f, 0.3f, 1.f })
		)
	);
```

#### _Launch_

This library comes with everything needed to create a native window and a hardware accelerated renderer using [SDL2]() and [Diligent Engine]() on the most common platforms, such as Windows, MacOS, Linux, iOS, Android and Emscripten. If you want to use this library directly without binding with an existing game/engine, the `staticgui::launch` function starts a window, a renderer, and blocks the thread until the window is closed. 

```
staticgui::launch(my_widget_tree);
```

You can define a function that will be called after the renderer is initialized and before the update loop begins. This can be useful to setup custom state that requires the window or the renderer to be initialized.

```
staticgui::launch(my_widget_tree, [&] () {
	// this code will be executed after renderer is initialized
	// but before update loop starts
});
```

This library behaves by 



#### _Embed_

You may want to use this library to design guis along with an existing game/engine. As we use [ImGui](https://github.com/ocornut/imgui) as a backend for rendering geometry and collecting input events, we can initialize staticgui before starting a custom game loop after ImGui has created a context with the `staticgui::embed` function. 

```
ImGui::CreateContext();
auto my_update_callback = staticgui::embed(my_widget_tree);
```

It takes our widget tree as input and returns a callback we can call between our `ImGui::NewFrame` and `ImGui::Render` custom game/engine calls. 

```
ImGui::NewFrame();
my_update_callback();
ImGui::Render();
```

You may not care about the battery cost of swapping buffers each frame, especially if you are already implementing 3D rendering. However the staticgui update callback given by `staticgui::embed` will by default only render geometry where visual changes have to be redrawn. This is useful because we can bind a framebuffer, call our embed callback that may or may not clear or repaint it, and blit the framebuffer at the end of the frame.

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

No macro is required inside widget classes, but the user is not allowed to own them. They must be constructed with the `staticgui::create` function which forwards all the arguments required for constructor invocation. All the widgets are implemented inside the `staticgui::widgets` namespace.

```
auto& my_widget = staticgui::create(staticgui::widgets::center_widget(...));
```

As this requires a looot of typing, widgets implementations use the __widget_ suffix and define an alias of `staticgui::create<widget_t>` inside the `staticgui` namespace. Then we can instead do this :

```
auto& my_widget = staticgui::center(...);
```

On construction, widgets must build their children. The `staticgui::build` function will usually be called by each constructor. Widget inheritance is possible, and widgets can be declared more than once (explain here...).
You are really encouraged to take advantage of template deduction when implementing generic widgets
if you reaaally need virtual inheritance among widget classes see the [CRTP idiom]()

```
my_widget::my_widget() {
	staticgui::build(this, my_child_widget_type());
}
```

Widgets can build template children by templating their constructors and accepting children as non-const references. Templating the widget struct would allow storing pointers or references to its children and having its members access them, but this is not always desirable. Depending on the use case this would prevent some useful usage of the `staticgui::context::iterate` function. For example the widgets `my_widget_1<float>` and `my_widget_1<int>` would not be iterable at the same time.

```
template <typename... children_widgets_t>
my_widget::my_widget(children_widgets_t&... my_other_children_widgets) {
	staticgui::build(this, my_child_widget_type(), my_other_children_widgets...);
}
```




#### _Resolve_

#### _Draw_

#### _Interact_


#### _Events_

To dynamically modify the gui state, event objects register callbacks of the same type and can be passed from a widget to its children to be triggered all at once when desired.

```
staticgui::event<float, float, std::string> my_event;
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

attach/detach



#### _Animations_

### __Limitations__

images fonts shaders etc


### __External dependencies__

- [bezier](https://github.com/oysteinmyrmo/bezier) [___MIT licence___] is
- [vectorclass](https://github.com/vectorclass/version2) [___Apache 2.0 licence___]
- [entt](https://github.com/skypjack/entt) [___MIT licence___]
- [imgui](https://github.com/ocornut/imgui) [___MIT licence___]
