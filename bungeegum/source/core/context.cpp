#pragma once

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/context.fwd>
#include <bungeegum/core/event.hpp>
#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.fwd>
#include <bungeegum/glue/curve.hpp>

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
        // bool _has_window_been_resized = false;

        // resolve si besoin
        // bool _must_draw = (!widgets.is_must_resolve_empty() && !widgets.is_must_draw_empty());

        // frames_chronometer.begin("resolve");
        bool _must_draw = (!widgets_context.is_must_draw_empty());
        widgets_context.iterate_must_resolve([&](untyped_widget_data& _data, const bool _must_resolve_children) {
            (void)_must_resolve_children;
            _data.widget_resolver_data.value().resolved_size = _data.widget_resolver(_data.widget_resolver_data.value());
        });
        // frames_chronometer.end("resolve");

        return _must_draw;
        // return true;
    }

    void draw()
    {

        // frames_chronometer.begin("draw");
        // widgets_context.iterate_must_draw([](untyped_widget_data& _data, const bool _must_draw_children) {
        //     (void)_must_draw_children;
        //     if (!has_userspace_thrown()) {
        //         std::function<void(untyped_widget_data&)> _ff = [&](untyped_widget_data& _data2) {
        //             if (_data2.drawer_command.has_value())
        //                 _data2.drawer_command.value()._data.commands.clear();
        //             if (_data2.drawer)
        //                 _data2.drawer(_data2.drawer_command.value());
        //             for (auto& _child_data : _data2.children)
        //                 _ff(_child_data.get());
        //         };
        //         _ff(_data);
        //     }
        //     protect_userspace([&]() {
        //         std::function<void(untyped_widget_data&)> _ff = [&](untyped_widget_data& _data2) {
        //             if (_data2.drawer && _data2.drawer_command.has_value())
        //                 _data2.drawer(_data2.drawer_command.value());
        //             for (auto& _child_data : _data2.children)
        //                 _ff(_child_data.get());
        //         };
        //         if (_data.interactor_command.has_value())
        //             _data.widget_interactor(_data.interactor_command.value()); // TEST INTERACT
        //         // _data.drawer(_data.resolve_command.value().resolved_size, _data.command.value());
        //         // _data.drawer(_data.widget_resolver_data.value().resolved_size, _data.widget_drawer_data.value());
        //         // _data.drawer({ 500.f, 500.f }, _data.widget_drawer_data.value());
        //         _ff(_data);
        //     });
        // });
        // frames_chronometer.end("draw");

        widgets_context.iterate([=](untyped_widget_data& _data) {
            if (_data.drawer_command.has_value()) {
                _data.drawer_command.value()._data.commands.clear();
                _data.drawer(_data.drawer_command.value());
            }
        });

        if constexpr (true) {
            draw_overlay([&](ImDrawList* _imgui_drawlist) {
                widgets_context.iterate([=](untyped_widget_data& _data) {
                    if (_data.drawer_command.has_value()) {
                        _data.drawer_command.value()._data.draw(_imgui_drawlist);
                    }
                });
            });
        } else {
        }

        if (!has_userspace_thrown()) {
            // widgets_context.clear_resolve();
            // widgets_context.clear_draw();
        }
    }
}
}