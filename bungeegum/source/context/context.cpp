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

#include <bungeegum/context/animation.hpp>
#include <bungeegum/context/context.hpp>
#include <bungeegum/context/event.hpp>
#include <bungeegum/context/exceptions.hpp>
#include <bungeegum/context/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/curve.hpp>
#include <bungeegum/overlay/overlay.hpp>

namespace bungeegum {
namespace detail {

    bool tick(const std::chrono::milliseconds& delta_time)
    {
        // frames_chronometer.new_frame();

        // frames_chronometer.begin("animations");

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        animations_context.tick(delta_time);
        // frames_chronometer.end("animations");

        // frames_chronometer.begin("events");
        events_context.tick();
        // frames_chronometer.end("events");

        // on window resize on resolve
        bool _has_window_been_resized = false;

        // resolve si besoin
        // bool _must_draw = (!widgets.is_must_resolve_empty() && !widgets.is_must_draw_empty());

        // frames_chronometer.begin("resolve");
        bool _must_draw = (!widgets_context.is_must_draw_empty());
        widgets_context.iterate_must_resolve([&](untyped_widget_data& _data, const bool _must_resolve_children) {
            _data.widget_resolver_data.value().resolved_size = _data.widget_resolver(_data.widget_resolver_data.value());
        });
        // frames_chronometer.end("resolve");

        return _must_draw;
        // return true;
    }

    void draw()
    {

        // frames_chronometer.begin("draw");
        widgets_context.iterate_must_draw([](untyped_widget_data& _data, const bool _must_draw_children) {
            if (!has_userspace_thrown()) {
                std::function<void(untyped_widget_data&)> _ff = [&](untyped_widget_data& _data) {
                    if (_data.widget_drawer_data.has_value())
                        _data.widget_drawer_data.value().commands.clear();
                    for (auto& _child_data : _data.children)
                        _ff(_child_data.get());
                };
                _ff(_data);
            }
            protect_userspace([&]() {
                std::function<void(untyped_widget_data&)> _ff = [&](untyped_widget_data& _data) {
                    if (_data.widget_drawer)
                        _data.widget_drawer({ 500.f, 500.f }, _data.widget_drawer_data.value());
                    for (auto& _child_data : _data.children)
                        _ff(_child_data.get());
                };
                // _data.drawer(_data.resolve_command.value().resolved_size, _data.command.value());
                // _data.widget_drawer(_data.widget_resolver_data.value().resolved_size, _data.widget_drawer_data.value());
                // _data.widget_drawer({ 500.f, 500.f }, _data.widget_drawer_data.value());
                _ff(_data);
            });
        });
        // frames_chronometer.end("draw");

        if constexpr (true) {
            draw_overlay([&](ImDrawList* _imgui_drawlist) {
                widgets_context.iterate([=](untyped_widget_data& _data) {
                    if (_data.widget_drawer_data.has_value()) {
                        _data.widget_drawer_data.value().draw(_imgui_drawlist);
                    }
                });
            });
        } else {
        }

        if (!has_userspace_thrown()) {
            widgets_context.clear_resolve();
            widgets_context.clear_draw();
        }
    }
}
}