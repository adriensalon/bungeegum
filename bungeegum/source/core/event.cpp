#pragma once

#include <bungeegum/core/event.hpp>

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
