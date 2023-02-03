# staticgui

This repository provides a basic implementation for a gui library that repaint only needed elements when needed with gpu acceleration, with a declarative syntax similar to flutter widgets, but relying on templates to achieve static polymorphism. The doxygen-generated documentation can be found [here](https://okok.org/) or built along the library.

### __Requirements__
Windows/MacOS/iOS/Linux/Android platform, but only tested on Windows so far

- a C++17 compiler is required
- [CMake](https://cmake.org/) build files are provided to facilitate the compilation

### __Features__
#### _Flutter-like declarative formalism_
#### _No policy is the best policy_
#### _Events_
#### _Animations_

### __Quickstart__

Launch / Attach





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


On resolve / On render


### __External dependencies__
