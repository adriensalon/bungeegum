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
#include <implot.h>
#include <iostream>

#include <staticgui/glue/backtrace.hpp>
#include <staticgui/glue/bspline.hpp>
#include <staticgui/overlay/overlay.hpp>
#include <staticgui/state/context.hpp>
#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {

    bool context_state::tick(const float delta_milliseconds)
    {
        frames_chronometer.new_frame();

        frames_chronometer.begin("animations");

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        animations.tick(delta_milliseconds);
        frames_chronometer.end("animations");

        frames_chronometer.begin("events");
        events.tick();
        frames_chronometer.end("events");

        // on window resize on resolve
        bool _has_window_been_resized = false;

        // resolve si besoin
        // bool _must_draw = (!widgets.is_must_resolve_empty() && !widgets.is_must_draw_empty());

        frames_chronometer.begin("resolve");
        bool _must_draw = (!widgets.is_must_draw_empty());
        widgets.iterate_must_resolve([&](widget_data& _data, const bool _must_resolve_children) {
            _data.resolve_command.value().resolved_size = _data.resolver(_data.resolve_command.value());
        });
        frames_chronometer.end("resolve");

        return _must_draw;
        // return true;
    }

    void context_state::draw()
    {

        frames_chronometer.begin("draw");
        widgets.iterate_must_draw([](widget_data& _data, const bool _must_draw_children) {
            if (!has_userspace_thrown())
                _data.command.value().clear();
            protect_userspace([&]() {
                // _data.drawer(_data.resolve_command.value().resolved_size, _data.command.value());
                _data.drawer(_data.resolve_command.value().resolved_size, _data.command.value());
            });
        });
        frames_chronometer.end("draw");

        if constexpr (true) {
            overlay::draw_overlay(*this, [&](ImDrawList* _imgui_drawlist) {
                widgets.iterate_datas([&](widget_data& _data) {
                    if (_data.command.has_value())
                        _data.command.value().draw(_imgui_drawlist);
                });
            });
        } else {
        }

        if (!has_userspace_thrown()) {
            widgets.clear_resolve();
            widgets.clear_draw();
        }
    }
}
}