#pragma once

#include <execution>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/context.fwd>
#include <bungeegum/core/event.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    void context::interact(const std::chrono::milliseconds& delta_time)
    {
        (void)delta_time;
        bool _interact_done = false;
        while (!_interact_done) {
            std::for_each(
                std::execution::seq, // go parallel
                widgets_context.interactables.begin(),
                widgets_context.interactables.end(),
                [](auto&& _data_reference) {
                    untyped_widget_data& _data = _data_reference.get();
                    if (_data.interactor_command.has_value())
                        _data.interactor(_data.interactor_command.value());
                });
            widgets_context.resolvables.erase(widgets_context.resolvables.begin(), widgets_context.resolvables.end());
            _interact_done = widgets_context.resolvables.empty();
        }
    }

    void context::resolve(const std::chrono::milliseconds& delta_time)
    {
        (void)delta_time;
        bool _resolve_done = false;
        while (!_resolve_done) {
            std::for_each(
                std::execution::seq, // go parallel
                widgets_context.resolvables.begin(),
                widgets_context.resolvables.end(),
                [](auto&& _data_reference) {
                    untyped_widget_data& _data = _data_reference.get();
                    if (_data.resolver_command.has_value())
                        _data.resolver(_data.resolver_command.value());
                });
            widgets_context.resolvables.erase(widgets_context.resolvables.begin(), widgets_context.resolvables.end());
            _resolve_done = widgets_context.resolvables.empty();
        }
    }

    void context::draw(ImDrawList* imgui_drawlist)
    {
        bool _draw_done = false;
        while (!_draw_done) {
            std::for_each(
                std::execution::seq, // go parallel
                widgets_context.drawables.begin(),
                widgets_context.drawables.end(),
                [imgui_drawlist](auto&& _data_reference) {
                    untyped_widget_data& _data = _data_reference.get();
                    if (_data.drawer_command.has_value()) {
                        _data.drawer_command.value()._data.commands.clear();
                        _data.drawer(_data.drawer_command.value());
                        _data.drawer_command.value()._data.draw(imgui_drawlist);
                    }
                });
            widgets_context.drawables.erase(widgets_context.drawables.begin(), widgets_context.drawables.end());
            _draw_done = widgets_context.drawables.empty();
        }
    }

    bool tick(const std::chrono::milliseconds& delta_time)
    {
        animations_context.tick(delta_time);
        events_context.tick();
        context::interact(delta_time);
        context::resolve(delta_time);
        return (!widgets_context.drawables.empty());
    }

    void draw()
    {
        draw_overlay(context::draw);
    }
}
}