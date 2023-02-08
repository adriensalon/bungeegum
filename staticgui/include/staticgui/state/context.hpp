//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/time.hpp>
#include <staticgui/state/animatable.hpp>
#include <staticgui/state/layout.hpp>
#include <staticgui/state/rendering.hpp>
#include <staticgui/state/widget.hpp>

#include <staticgui/state/event.hpp>
namespace staticgui {
namespace detail {

    struct context_state {

        bool tick(const float delta_milliseconds);
        void draw();

        widget_registry widgets;
        animation_registry animations;
        event_registry events;

        unsigned int max_fps = 60;
        glue::chronometer<10, std::chrono::milliseconds> frames_chronometer;
    };

}
}
