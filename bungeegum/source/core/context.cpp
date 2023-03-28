#pragma once

#include <execution>
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
        animations_context.tick(delta_time);
        events_context.tick();

        return true;
    }

    void interact()
    {
        bool _interact_done = false;
        while (!_interact_done) {
            std::for_each(
                std::execution::seq, // go parallel
                widgets_context.drawables.begin(),
                widgets_context.drawables.end(),
                [](auto&& _data_reference) {
                    untyped_widget_data& _data = _data_reference.get();
                    if (_data.interactor_command.has_value())
                        _data.interactor(_data.interactor_command.value()); // tick = true ?
                });
            widgets_context.resolvables.erase(widgets_context.resolvables.begin(), widgets_context.resolvables.end());
            _interact_done = widgets_context.resolvables.empty();
        }
    }

    void resolve()
    {
        bool _resolve_done = false;
        while (!_resolve_done) {
            std::for_each(
                std::execution::seq, // go parallel
                widgets_context.drawables.begin(),
                widgets_context.drawables.end(),
                [](auto&& _data_reference) {
                    untyped_widget_data& _data = _data_reference.get();
                    if (_data.resolver_command.has_value())
                        _data.resolver(_data.resolver_command.value()); // tick = true ?
                });
            widgets_context.resolvables.erase(widgets_context.resolvables.begin(), widgets_context.resolvables.end());
            _resolve_done = widgets_context.resolvables.empty();
        }
    }

    void draw()
    {
        draw_overlay([](ImDrawList* _imgui_drawlist) {
            bool _draw_done = false;
            while (!_draw_done) {
                std::for_each(
                    std::execution::seq, // go parallel
                    widgets_context.drawables.begin(),
                    widgets_context.drawables.end(),
                    [_imgui_drawlist](auto&& _data_reference) {
                        untyped_widget_data& _data = _data_reference.get();
                        if (_data.drawer_command.has_value()) {
                            _data.drawer_command.value()._data.commands.clear();
                            _data.drawer(_data.drawer_command.value());
                            _data.drawer_command.value()._data.draw(_imgui_drawlist);
                        }
                    });
                widgets_context.drawables.erase(widgets_context.drawables.begin(), widgets_context.drawables.end());
                _draw_done = widgets_context.drawables.empty();
            }
        });
    }
}
}