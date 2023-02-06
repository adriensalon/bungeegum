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
        return *this;
    }

    void event_registry::tick()
    {
        iterate_datas([this](event_data& _event_data) {
            _event_data.tick();
        });
    }

    void event_registry::iterate_datas(const std::function<void(event_data&)>& iterate_callback)
    {
        _registry.iterate_components<event_data>(iterate_callback);
    };
}
}
