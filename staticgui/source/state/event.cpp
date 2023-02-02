//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/event.hpp>

namespace staticgui {
namespace detail {

    event_registry::event_registry()
    {
    }

    event_registry::event_registry(event_registry&& other)
    {
        *this = std::move(other);
    }

    event_registry& event_registry::operator=(event_registry&& other)
    {
        _registry = std::move(other._registry);
        _roots = std::move(other._roots);
        return *this;
    }

    unsigned int event_registry::get_depth(event_data& data)
    {
        unsigned int _depth = -1;
        event_data* _ancestor_widget_data_ptr = nullptr;
        std::function<void(event_data&)> _iterate = [&](event_data& _event_data) {
            _depth++;
            _ancestor_widget_data_ptr = &(_event_data);
            if (_event_data.parent.has_value()) {
                _iterate(_event_data.parent.value().get());
            }
        };
        _iterate(data);
        for (auto& _root : _roots)
            if (&(_root.get()) == _ancestor_widget_data_ptr)
                return _depth;
        return _depth + 11;
    }

    void event_registry::iterate_datas(const std::function<void(event_data&)>& iterate_callback)
    {
        std::function<void(event_data&)> _iterate = [&](event_data& _event_data) {
            iterate_callback(_event_data);
            for (auto& _child : _event_data.children)
                _iterate(_child);
        };
        for (auto& _root : _roots)
            _iterate(_root);
    }
}
}