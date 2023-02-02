//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/animations_registry.hpp>
#include <staticgui/state/events_registry.hpp>
#include <staticgui/state/widgets_registry.hpp>

namespace staticgui {
namespace detail {

    struct context_state {

        template <typename widget_t>
        void build_root(widget_t& widget);

        bool tick_all(const float delta_milliseconds);

    private:
        animations_registry _animations;
        events_registry _events;
        widgets_registry _widgets;
    };

}
}

#include <staticgui/state/context_state.inl>