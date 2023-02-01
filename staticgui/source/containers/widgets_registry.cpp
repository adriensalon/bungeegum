//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/containers/widgets_registry.hpp>

namespace staticgui {
namespace detail {

    widgets_registry::widgets_registry()
    {
    }

    widgets_registry::widgets_registry(widgets_registry&& other)
    {
        *this = std::move(other);
    }

    widgets_registry& widgets_registry::operator=(widgets_registry&& other)
    {
        _registry = std::move(other._registry);
        _roots = std::move(other._roots);
        return *this;
    }

    unsigned int widgets_registry::get_depth(widget_data& data)
    {
        unsigned int _depth = -1;
        std::function<const widget_data*(const widget_data*)> _iterate = [&](const widget_data* _widget_data) {
            _depth++;
            return _widget_data->parent ? _iterate(_widget_data->parent) : _widget_data;
        };
        const widget_data* _ancestor = _iterate(&data);
        widget_data* _root_ancestor = nullptr;
        for (auto& _root : _roots)
            if (&(_root.get()) == _ancestor)
                return _depth;
        return _depth + 11;
    }

    void widgets_registry::iterate_datas(const std::function<void(widget_data&)>& iterate_callback)
    {
        std::function<void(widget_data*)> _iterate = [&](widget_data* _widget_data) {
            iterate_callback(*_widget_data);
            for (auto* _child : _widget_data->children)
                _iterate(_child);
        };
        for (auto& _root : _roots)
            _iterate(&(_root.get()));
    }

}
}