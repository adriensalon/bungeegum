#include <execution>
#include <iostream>

#include <SDL.h>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/context.fwd>
#include <bungeegum/core/event.hpp>
#include <bungeegum/core/exceptions.fwd>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    void context::execute_interact()
    {
#define traverse_interact_impl(interaction_name)                                                                    \
    for (const interaction_name##_interaction_data& _interaction : interaction_name##_interactions) {               \
        traverse_untyped_widgets(widgets_context.root.value(), [&_interaction](untyped_widget_data& _widget_data) { \
            if (_widget_data.interactor_command.has_value()) {                                                      \
                _widget_data.interactor_command.value()._data.is_blocked = false;                                   \
                _widget_data.interactor_command.value()._data.command_data = _interaction;                          \
                protect_userspace([&_widget_data]() {                                                               \
                    _widget_data.interactor(_widget_data.interactor_command.value());                               \
                });                                                                                                 \
                bool _retval = (!_widget_data.interactor_command.value()._data.is_blocked);                         \
                return _retval;                                                                                     \
            }                                                                                                       \
            return true;                                                                                            \
        });                                                                                                         \
    }                                                                                                               \
    interaction_name##_interactions.clear();

        traverse_interact_impl(window_resized);
        traverse_interact_impl(mouse_moved);
        traverse_interact_impl(mouse_down);
        traverse_interact_impl(mouse_up);
        traverse_interact_impl(mouse_pressed);

#undef traverse_interact_impl
    }

    void context::execute_resolve(const std::chrono::milliseconds& delta_time)
    {
        (void)delta_time;
        bool _resolve_done = false;
        while (!_resolve_done) {
            std::for_each(
                std::execution::seq, // go parallel
                widgets_context.resolvables.begin(),
                widgets_context.resolvables.end(),
                [](auto&& _data_reference) {
                    untyped_widget_data& _widget_data = _data_reference.get();
                    if (_widget_data.resolver_command.has_value()) {
                        protect_userspace([&_widget_data]() {
                            _widget_data.resolver(_widget_data.resolver_command.value());
                        });
                    }
                });
            widgets_context.resolvables.erase(widgets_context.resolvables.begin(), widgets_context.resolvables.end());
            _resolve_done = widgets_context.resolvables.empty();
        }
    }

    void context::execute_draw(ImDrawList* imgui_drawlist, const bool heads_start)
    {
        bool _draw_done = false;
        if (heads_start) {
            // todo
        } else {
            while (!_draw_done) {
                std::for_each(
                    std::execution::seq, // go parallel
                    widgets_context.drawables.begin(),
                    widgets_context.drawables.end(),
                    [imgui_drawlist](auto&& _data_reference) {
                        untyped_widget_data& _widget_data = _data_reference.get();
                        if (_widget_data.drawer_command.has_value()) {
                            _widget_data.drawer_command.value()._data.commands.clear();
                            protect_userspace([&_widget_data]() {
                                _widget_data.drawer(_widget_data.drawer_command.value());
                            });
                            _widget_data.drawer_command.value()._data.draw(imgui_drawlist);
                        }
                    });
                widgets_context.drawables.erase(widgets_context.drawables.begin(), widgets_context.drawables.end());
                _draw_done = widgets_context.drawables.empty();
            }
        }
    }

    void process_input(const std::any& input)
    {
        SDL_Event _event = std::any_cast<SDL_Event>(input);
        const SDL_Event* _event_ptr = &_event;
        if ((_event_ptr->type == SDL_WINDOWEVENT) && (_event_ptr->window.event == SDL_WINDOWEVENT_RESIZED)) {
            window_resized_interaction_data _interaction_data;
            _interaction_data.new_size = float2 { 0.f, 0.f };
            _interaction_data.is_fullscreen = false;
            context::window_resized_interactions.emplace_back(_interaction_data);
            // int _w, _h;
            // SDL_GetWindowSize(get(_diligent_renderer).sdl_window, &_w, &_h);
            // get(_diligent_renderer).swap_chain->Resize(_w, _h);
        } else if (_event_ptr->type == SDL_MOUSEMOTION) {
            mouse_moved_interaction_data _interaction_data;
            _interaction_data.absolute_position = float2 { _event_ptr->motion.x, _event_ptr->motion.y };
            _interaction_data.relative_position = float2 { _event_ptr->motion.xrel, _event_ptr->motion.yrel };
            context::mouse_moved_interactions.emplace_back(_interaction_data);
        } else if (_event_ptr->type == SDL_MOUSEBUTTONDOWN) {
            mouse_down_interaction_data _interaction_data;
            _interaction_data.button_index = static_cast<unsigned int>(_event_ptr->button.button);
            context::mouse_down_interactions.emplace_back(_interaction_data);
        } else if (_event_ptr->type == SDL_MOUSEBUTTONUP) {
            mouse_up_interaction_data _interaction_data;
            _interaction_data.button_index = static_cast<unsigned int>(_event_ptr->button.button);
            context::mouse_up_interactions.emplace_back(_interaction_data);
        }
        // pressed
    }

    bool tick(const std::chrono::milliseconds& delta_time)
    {
        animations_context.tick(delta_time);
        detail::events_context.events.iterate<detail::untyped_event_data>([](detail::untyped_event_data& _event_data) {
            _event_data.ticker();
        });
        context::execute_interact();
        context::execute_resolve(delta_time);
        return (has_userspace_thrown() || !widgets_context.drawables.empty());
    }

    void draw(const bool heads_start)
    {
        draw_overlay([heads_start](ImDrawList* imgui_drawlist) {
            context::execute_draw(imgui_drawlist, heads_start);
        });
    }
}
}