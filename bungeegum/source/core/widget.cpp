#pragma once

#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    void traverse_untyped_widgets(untyped_widget_data& iterate_root, const std::function<bool(untyped_widget_data&)>& iterate_callback)
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