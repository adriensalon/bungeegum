#include <execution>
#include <iostream>

#include <SDL.h>

#include <bungeegum/backend/common.fwd>
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
#define traverse_interact_impl(interaction_name)                                                                      \
    for (const interaction_name##_event& _event : interaction_name##_events) {                                        \
        widgets_context.traverse_untyped(widgets_context.root.value(), [&_event](untyped_widget_data& _widget_data) { \
            if (_widget_data.interactor_command.has_value()) {                                                        \
                _widget_data.interactor_command.value()._data.is_blocked = false;                                     \
                _widget_data.interactor_command.value()._data.command_data = _event;                                  \
                protect_userspace([&_widget_data]() {                                                                 \
                    _widget_data.interactor(_widget_data.interactor_command.value());                                 \
                });                                                                                                   \
                bool _retval = (!_widget_data.interactor_command.value()._data.is_blocked);                           \
                return _retval;                                                                                       \
            }                                                                                                         \
            return true;                                                                                              \
        });                                                                                                           \
    }                                                                                                                 \
    interaction_name##_events.clear();

        traverse_interact_impl(window_resized);
        traverse_interact_impl(mouse_moved);
        traverse_interact_impl(mouse_down);
        traverse_interact_impl(mouse_up);
        traverse_interact_impl(mouse_pressed);

#undef traverse_interact_impl
    }

    void context::execute_resolve()
    {
        bool _resolve_done = false;
        while (!_resolve_done) {
            std::for_each(
                std::execution::seq, // go parallel
                widgets_context.resolvables.begin(),
                widgets_context.resolvables.end(),
                [](auto&& _data_reference) {
                    untyped_widget_data& _widget_data = _data_reference.get();
                    resolve_command& _resolve_command = _widget_data.resolver_command.value();

                    if (_widget_data == widgets_context.root.value().get()) {
                        _resolve_command._data.constraint.min_size = detail::viewport_size;
                        _resolve_command._data.constraint.max_size = detail::viewport_size;
                    } else {
                        untyped_widget_data* _parent_widget_data_ptr = &_widget_data.parent.value().get();
                        bool _root_is_parent = false;
                        while (!_parent_widget_data_ptr->resolver_command.has_value()) {
                            if (*_parent_widget_data_ptr == widgets_context.root.value().get()) {
                                _resolve_command._data.constraint.min_size = detail::viewport_size;
                                _resolve_command._data.constraint.max_size = detail::viewport_size;
                                std::cout << "viewport size = " << detail::viewport_size.x << ", " << detail::viewport_size.y << std::endl;
                                _root_is_parent = true;
                                break;
                            }
                            _parent_widget_data_ptr = &_parent_widget_data_ptr->parent.value().get();
                        }
                        if (!_root_is_parent) {
                            resolve_command& _parent_resolve_command = _parent_widget_data_ptr->resolver_command.value();
                            _resolve_command._data.constraint.min_size = _parent_resolve_command.min_size();
                            _resolve_command._data.constraint.max_size = _parent_resolve_command.max_size();
                        }
                        // _resolve_command._data.resolved_position = _parent_widget_data_ptr->resolver_command.value()._data.resolved_position;
                    }

                    protect_userspace([&_widget_data]() {
                        _widget_data.resolver(_widget_data.resolver_command.value());
                    });

                    // float2 _resolved_size = _resolve_command._data.resolved_size;
                    // if (_widget_data.drawer_command.has_value()) {
                    //     draw_command& _draw_command = _widget_data.drawer_command.value();
                    //     _draw_command._data.resolved_size = _resolved_size;
                    // }
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
                        untyped_widget_data& _drawable_widget_data = _data_reference.get();
                        widgets_context.traverse_untyped(_drawable_widget_data, [imgui_drawlist](untyped_widget_data& _widget_data) {
                            // accumulate position anyway
                            resolve_command& _widget_resolver_command = _widget_data.resolver_command.value();
                            if (_widget_data.parent.has_value()) {
                                resolve_command& _parent_resolver_command = _widget_data.parent.value().get().resolver_command.value();
                                _widget_resolver_command._data.accumulated_position = _widget_resolver_command._data.resolved_position + _parent_resolver_command._data.accumulated_position;
                            }

                            // draw
                            if (_widget_data.drawer_command.has_value()) {

                                draw_command& _widget_drawer_command = _widget_data.drawer_command.value();
                                _widget_drawer_command._data.resolved_size = _widget_resolver_command._data.resolved_size;
                                _widget_drawer_command._data.resolved_position = _widget_resolver_command._data.accumulated_position;
                                _widget_drawer_command._data.commands.clear();
                                protect_userspace([&_widget_data, &_widget_drawer_command]() {
                                    _widget_data.drawer(_widget_drawer_command);
                                });
                                _widget_drawer_command._data.draw(imgui_drawlist);
                            }
                            return true;
                        });
                    });
                widgets_context.drawables.erase(widgets_context.drawables.begin(), widgets_context.drawables.end());
                _draw_done = widgets_context.drawables.empty();
            }
        }
    }

    // void process_input(const std::any& input)
    // {
    //     SDL_Event _event = std::any_cast<SDL_Event>(input);
    //     const SDL_Event* _event_ptr = &_event;
    //     if ((_event_ptr->type == SDL_WINDOWEVENT) && (_event_ptr->window.event == SDL_WINDOWEVENT_RESIZED)) {
    //         window_resized_interaction_data _interaction_data;
    //         _interaction_data.new_size = detail::viewport_size;
    //         _interaction_data.is_fullscreen = false; // TODO
    //         context::window_resized_interactions.emplace_back(_interaction_data);
    //         // int _w, _h;
    //         // SDL_GetWindowSize(get(_diligent_renderer).sdl_window, &_w, &_h);
    //         // get(_diligent_renderer).swap_chain->Resize(_w, _h);
    //         // must_resolve();
    //         // must_draw();
    //     } else if (_event_ptr->type == SDL_MOUSEMOTION) {
    //         mouse_moved_interaction_data _interaction_data;
    //         _interaction_data.absolute_position = float2 { _event_ptr->motion.x, _event_ptr->motion.y };
    //         _interaction_data.relative_position = float2 { _event_ptr->motion.xrel, _event_ptr->motion.yrel };
    //         context::mouse_moved_interactions.emplace_back(_interaction_data);
    //     } else if (_event_ptr->type == SDL_MOUSEBUTTONDOWN) {
    //         mouse_down_interaction_data _interaction_data;
    //         _interaction_data.button_index = static_cast<unsigned int>(_event_ptr->button.button);
    //         context::mouse_down_interactions.emplace_back(_interaction_data);
    //     } else if (_event_ptr->type == SDL_MOUSEBUTTONUP) {
    //         mouse_up_interaction_data _interaction_data;
    //         _interaction_data.button_index = static_cast<unsigned int>(_event_ptr->button.button);
    //         context::mouse_up_interactions.emplace_back(_interaction_data);
    //     }
    //     // pressed
    // }

    // TODO PROCESS EACH HERE

    bool tick(const std::chrono::milliseconds& delta_time)
    {
        global_animations_manager.update(delta_time);
        global_events_manager.update();
        ////
        ////
        ////

        // debug only !
        must_resolve();
        must_draw();
        // debug only !

        // context::execute_interact();
        context::execute_resolve();
        return (has_userspace_thrown() || !widgets_context.drawables.empty());
        // return true;
    }

    void draw(const bool heads_start)
    {
        draw_overlay([heads_start](ImDrawList* imgui_drawlist) {
            context::execute_draw(imgui_drawlist, heads_start);
        });
    }
}
}