//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <imgui.h>
#include <iostream>

#include <staticgui/state/context.hpp>
#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {

    bool context_state::tick(const float delta_milliseconds)
    {
        animations.tick(delta_milliseconds);
        events.tick();

        // on window resize on resolve
        bool _has_window_been_resized = false;

        // resolve si besoin
        // bool _must_draw = (!widgets.is_must_resolve_empty() && !widgets.is_must_draw_empty());
        bool _must_draw = (!widgets.is_must_draw_empty());
        widgets.iterate_must_resolve([&](widget_data& _data, const bool _must_resolve_children) {

        });
        widgets.clear_resolve();

        return _must_draw;
    }

    void context_state::draw()
    {
        // throw_library_bad_usage("whatt");
        //
        //
        //
        //
        widgets.iterate_must_draw([](widget_data& _data, const bool _must_draw_children) {
            _data.command.value().clear();
            protect_userspace([&]() {
                _data.drawer(_data.command.value());
            });
        });
        widgets.iterate_datas([](widget_data& _data) {
            if (_data.command.has_value())
                _data.command.value().draw();
        });
        if (!has_userspace_thrown())
            widgets.clear_draw();
    }
}
}