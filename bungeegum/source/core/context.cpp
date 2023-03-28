#pragma once

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/context.fwd>
#include <bungeegum/core/event.hpp>
#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/overlay.fwd>
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
        // bool _must_draw = (!widgets_context.drawables.empty());
        // for (auto& _must_resolve : widgets_context.resolvables) {
        //     auto& _data = _must_resolve.first.get();
        //     _data.widget_resolver_data.value().resolved_size = _data.widget_resolver(_data.widget_resolver_data.value());
        // };
        // frames_chronometer.end("resolve");

        return true;
        // return true;
    }

    void interact()
    {
        // 1 go impl ca
    }

    void resolve()
    {
        bool _resolve_done = false;
        while (!_resolve_done) {
            std::vector<std::reference_wrapper<untyped_widget_data>>::iterator _resolve_iterator;
            for (_resolve_iterator = widgets_context.resolvables.begin(); _resolve_iterator != widgets_context.resolvables.end(); _resolve_iterator++) {
                untyped_widget_data& _data = _resolve_iterator->get();
                if (_data.resolver_command.has_value())
                    _data.resolver(_data.resolver_command.value());
            }
            widgets_context.resolvables.erase(widgets_context.resolvables.begin(), widgets_context.resolvables.end());
            _resolve_done = widgets_context.resolvables.empty();
        }
    }

    void draw()
    {
        draw_overlay([](ImDrawList* _imgui_drawlist) {
            bool _draw_done = false;
            while (!_draw_done) {
                std::vector<std::reference_wrapper<untyped_widget_data>>::iterator _draw_iterator;
                for (_draw_iterator = widgets_context.drawables.begin(); _draw_iterator != widgets_context.drawables.end(); _draw_iterator++) {
                    untyped_widget_data& _data = _draw_iterator->get();
                    if (_data.drawer_command.has_value()) {
                        _data.drawer_command.value()._data.commands.clear();
                        _data.drawer(_data.drawer_command.value());
                        _data.drawer_command.value()._data.draw(_imgui_drawlist);
                    }
                }
                widgets_context.drawables.erase(widgets_context.drawables.begin(), widgets_context.drawables.end());
                _draw_done = widgets_context.drawables.empty();
            }
        });
    }
}
}