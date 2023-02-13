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

#### _Launch_



```
#include <staticgui/staticgui.hpp>

int main()
{
	
}


```



#### _Embed_

You may want to use this library to design guis along with an existing game/engine. As we use [ImGui](https://github.com/ocornut/imgui) as a backend for rendering geometry and collecting input events, we can initialize staticgui before starting a custom game loop after ImGui has created a context with the `staticgui::embed` function. 

```
ImGui::CreateContext();
auto update_callback = staticgui::embed(center(row(...));
```

It takes our widget tree as input and returns a callback we can call between our `ImGui::NewFrame` and `ImGui::Render` custom game/engine calls. 

```
ImGui::NewFrame();
update_callback();
ImGui::Render();
```

You may not care about the battery cost of swapping buffers each frame, especially if you are already implementing 3D rendering. However the staticgui update callback given by `staticgui::embed` will by default only render geometry where visual changes have to be redrawn. This is useful because we can bind a framebuffer, call our embed callback that may or may not clear or repaint it, and blit the framebuffer at the end of the frame.

```
ImGui::NewFrame();
bind_framebuffer(); // we use a framebuffer to store the gui
update_callback();
blit_framebuffer_to_screen();
ImGui::Render();
```

If you dont want to do this and instead force redrawing the whole gui every frame, just pass `true` to the embed callback.

```
ImGui::NewFrame();
update_callback(bool); // instead we just force draw every frame
ImGui::Render();
```


#### _Events_

#### _Animations_
Launch / Attach

events
animations

### __Advanced usage__

#### _Layout_

#### _Rendering_

#### _Input_

### __Limitations__

No macro is required inside the widget types, but the user is not allowed to own them. They are created with the `staticgui::make` function which forwards all the template arguments required for custom constructor invocation. All the widgets are implemented inside the `staticgui::widgets` namespace.

```
auto& my_widget = staticgui::make(staticgui::widgets::center_widget(...))
```

As this requires a looot of typing, widgets implementations use the __widget_ suffix and define an alias of `staticgui::make<widget_t>(...)` inside the `staticgui` namespace. Then we can instead do this :

```
auto& my_widget = staticgui::center(...)
```

On construction, widgets must declare themselves along with their children, visible or not. The `staticgui::declare` function must be called by each constructor. Widget inheritance is possible, but widgets must be declared only once. All the `staticgui::context` functions must be called after the widget is declared.

```
my_widget::my_widget() {
	staticgui::declare(this, 
		my_other_widget(), 
		center(
			my_other_other_widget()
		)			
	);
}
```

They can declare `template` children by templating their constructors and accepting children as non-const references. Templating the widget struct would allow storing pointers or references to its children and having its members access them, but this is not always desirable. Depending on the use case this would prevent some useful usage of the `staticgui::context::iterate` function. For example the widgets `my_widget_1<float>` and `my_widget_1<int>` would not be iterable at the same time.

```
template <typename... children_widgets_t>
my_widget::my_widget(children_widgets_t&... children_widgets) {
	staticgui::declare(this, 
		my_other_widget_1(),
		children_widgets...
	);
}
```


### __External dependencies__

- [bezier](https://github.com/oysteinmyrmo/bezier) [___MIT licence___] is
- [vectorclass](https://github.com/vectorclass/version2) [___Apache 2.0 licence___]
- [entt](https://github.com/skypjack/entt) [___MIT licence___]
- [imgui](https://github.com/ocornut/imgui) [___MIT licence___]
