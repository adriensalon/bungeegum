# staticgui

This repository provides a basic implementation for a gui library that repaint only needed elements when needed with gpu acceleration, with a declarative syntax similar to flutter widgets, but relying on templates to achieve static polymorphism. The doxygen-generated documentation can be found [here](https://okok.org/) or built along the library.

### __Requirements to build__
Windows/MacOS/iOS/Linux/Android platform, but only tested on Windows so far

- a C++17 compiler is required
- [CMake](https://cmake.org/) build files are provided to facilitate the compilation

### __Features__
#### _Flutter-like formalism_

#### _No policy is the best policy_

#### _Build only when needed_

### __Quickstart__
Launch / Attach

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

### __Advanced usage__

#### _Layout_

#### _Rendering_


### __External dependencies__

- [bezier](https://github.com/oysteinmyrmo/bezier) [___MIT licence___] is
- [vectorclass](https://github.com/vectorclass/version2) [___Apache 2.0 licence___]
- [entt](https://github.com/skypjack/entt) [___MIT licence___]
- [imgui](https://github.com/ocornut/imgui) [___MIT licence___]
