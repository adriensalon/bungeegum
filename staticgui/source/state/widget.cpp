//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/widget.hpp>

namespace staticgui {
namespace detail {

    widget_registry::widget_registry()
    {
    }

    widget_registry::widget_registry(widget_registry&& other)
    {
        *this = std::move(other);
    }

    widget_registry& widget_registry::operator=(widget_registry&& other)
    {
        _registry = std::move(other._registry);
        _roots = std::move(other._roots);
        return *this;
    }

    unsigned int widget_registry::get_depth(widget_data& data)
    {
        unsigned int _depth = -1;
        widget_data* _ancestor_widget_data_ptr = nullptr;
        std::function<void(widget_data&)> _iterate = [&](widget_data& _widget_data) {
            _depth++;
            _ancestor_widget_data_ptr = &(_widget_data);
            if (_widget_data.parent.has_value()) {
                _iterate(_widget_data.parent.value().get());
            }
        };
        _iterate(data);
        for (auto& _root : _roots)
            if (&(_root.get()) == _ancestor_widget_data_ptr)
                return _depth;
        return _depth + 11;
    }

    void widget_registry::iterate_datas(const std::function<void(widget_data&)>& iterate_callback)
    {
        std::function<void(widget_data&)> _iterate = [&](widget_data& _widget_data) {
            iterate_callback(_widget_data);
            for (auto& _child : _widget_data.children)
                _iterate(_child);
        };
        for (auto& _root : _roots)
            _iterate(_root);
    }
}
}