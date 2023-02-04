//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/animation.hpp>
#include <staticgui/state/event.hpp>
#include <staticgui/state/layout.hpp>
#include <staticgui/state/rendering.hpp>
#include <staticgui/state/widget.hpp>

namespace staticgui {
namespace detail {

    struct context_state {

        bool tick(const float delta_milliseconds);
        void draw();

        widget_registry widgets;
        animation_registry animations;
        event_registry events;
    };

}
}
