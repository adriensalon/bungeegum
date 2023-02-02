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

        template <typename widget_t>
        void build_root(widget_t& widget);

        bool tick(const float delta_milliseconds);

        void draw();

        template <typename widget_t>
        void must_resolve(widget_t& widget, const bool must_resolve_children = true);

        template <typename widget_t>
        void must_draw(widget_t& widget, const bool must_draw_children = true);

    private:
        animation_registry _animations;
        event_registry _events;
        widget_registry _widgets;
    };

}
}

#include <staticgui/state/context.inl>