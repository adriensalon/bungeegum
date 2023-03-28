#pragma once

#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    void widgets_registry::traverse(untyped_widget_data& iterate_root, const untyped_widget_data_callback& iterate_callback)
    {
        std::function<void(untyped_widget_data&)> _iterate = [&](untyped_widget_data& _widget_data) {
            iterate_callback(_widget_data);
            for (auto& _child : _widget_data.children)
                _iterate(_child.get());
        };
        _iterate(iterate_root);
    }
}
}