//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <bungeegum/context/event.hpp>

namespace bungeegum {
namespace detail {

    void events_registry::tick()
    {
        iterate([this](untyped_event_data& _data) {
            _data.tick();
        });
    }

    void events_registry::iterate(const std::function<void(untyped_event_data&)>& iterate_callback)
    {
        events.iterate<untyped_event_data>(iterate_callback);
    };
}
}
