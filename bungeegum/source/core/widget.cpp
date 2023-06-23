#pragma once

#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    // runtime_widget widgets_registry::create_adopted(untyped_widget_data& untyped_widget)
    // {
    //     runtime_widget _adopted;
    //     _adopted._data.untyped_widget = untyped_widget;
    //     return _adopted;
    // }

    void widgets_registry::traverse_untyped(untyped_widget_data& iterate_root, const std::function<bool(untyped_widget_data&)>& iterate_callback)
    {
        std::function<void(untyped_widget_data&)> _iterate = [&](untyped_widget_data& _widget_data) {
            bool _continue = iterate_callback(_widget_data);
            if (_continue)
                for (auto& _child : _widget_data.children)
                    _iterate(_child.get());
        };
        _iterate(iterate_root);
    }
}
}