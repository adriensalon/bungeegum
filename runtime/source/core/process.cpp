#include <execution>

#include <bungeegum/backend/backend.fwd>
#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/event.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.fwd>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/process.fwd>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    void process_manager::_process_interact()
    {
#define traverse_interact_impl(interaction_name)                                                                           \
    for (const interaction_name##_event& _event : interaction_name##_events) {                                             \
        global_widgets_manager.traverse(global_widgets_manager.root.value(), [&_event](widget_update_data& _widget_data) { \
            if (_widget_data.interactor_command.has_value()) {                                                             \
                _widget_data.interactor_command.value()._data.is_blocked = false;                                          \
                _widget_data.interactor_command.value()._data.command_data = _event;                                       \
                global_manager::logs().protect_userspace([&_widget_data]() {                                               \
                    _widget_data.interactor(_widget_data.interactor_command.value());                                      \
                });                                                                                                        \
                bool _retval = (!_widget_data.interactor_command.value()._data.is_blocked);                                \
                return _retval;                                                                                            \
            }                                                                                                              \
            return true;                                                                                                   \
        });                                                                                                                \
    }                                                                                                                      \
    interaction_name##_events.clear();

        // traverse_interact_impl(window_resized);
        // traverse_interact_impl(mouse_moved);
        // traverse_interact_impl(mouse_down);
        // traverse_interact_impl(mouse_up);
        // traverse_interact_impl(mouse_pressed);

#undef traverse_interact_impl
    }

    void process_manager::_process_resolve()
    {
        bool _resolve_done = false;
        while (!_resolve_done) {
            std::for_each(
                std::execution::seq, // go parallel
                global_manager::widgets().resolvables.begin(),
                global_manager::widgets().resolvables.end(),
                [](const std::uintptr_t _raw_widget) {
                    widget_update_data& _widget_data = global_manager::widgets()[_raw_widget];
                    resolve_command& _resolve_command = _widget_data.resolver_command;
                    if (_raw_widget == global_manager::widgets().root()) {
                        _resolve_command._data.constraint.min_size = global_manager::backend().viewport_size;
                        _resolve_command._data.constraint.max_size = global_manager::backend().viewport_size;
                    } else {
                        widget_update_data* _parent_widget_data_ptr = &_widget_data.parent.value().get();

                        resolve_command& _parent_resolve_command = _parent_widget_data_ptr->resolver_command;
                        _resolve_command._data.constraint.min_size = _parent_resolve_command.min_size();
                        _resolve_command._data.constraint.max_size = _parent_resolve_command.max_size();
                    }
                    global_manager::logs().protect_userspace([&_widget_data]() {
                        _widget_data.resolver(_widget_data.resolver_command);
                    });
                });
            global_manager::widgets().resolvables.erase(
                global_manager::widgets().resolvables.begin(),
                global_manager::widgets().resolvables.end());
            _resolve_done = global_manager::widgets().resolvables.empty();
        }
    }

    void process_manager::_process_draw(ImDrawList* imgui_drawlist)
    {
        bool _draw_done = false;

        while (!_draw_done) {
            std::for_each(
                std::execution::seq, // go parallel
                global_manager::widgets().drawables.begin(),
                global_manager::widgets().drawables.end(),
                [imgui_drawlist](const std::uintptr_t _raw_widget) {
                    widget_update_data& _widget_data = global_manager::widgets()[_raw_widget];
                    global_manager::widgets().traverse(_widget_data, [imgui_drawlist](widget_update_data& _widget_data) {
                        // accumulate position anyway
                        resolve_command& _widget_resolver_command = _widget_data.resolver_command;
                        if (_widget_data.parent.has_value()) {
                            resolve_command& _parent_resolver_command = _widget_data.parent.value().get().resolver_command;
                            _widget_resolver_command._data.accumulated_position = _widget_resolver_command._data.resolved_position + _parent_resolver_command._data.accumulated_position;
                        }

                        // draw
                        if (_widget_data.drawer_command.has_value()) {

                            draw_command& _widget_drawer_command = _widget_data.drawer_command.value();
                            _widget_drawer_command._data.resolved_size = _widget_resolver_command._data.resolved_size;
                            _widget_drawer_command._data.resolved_position = _widget_resolver_command._data.accumulated_position;
                            _widget_drawer_command._data.commands.clear();
                            global_manager::logs().protect_userspace([&_widget_data, &_widget_drawer_command]() {
                                _widget_data.drawer(_widget_drawer_command);
                            });
                            _widget_drawer_command._data.draw(imgui_drawlist);
                        }
                        return true;
                    });
                });
            global_manager::widgets().drawables.erase(
                global_manager::widgets().drawables.begin(),
                global_manager::widgets().drawables.end());
            _draw_done = global_manager::widgets().drawables.empty();
        }
    }

    bool process_manager::update(const std::chrono::milliseconds& delta_time)
    {
        global_manager::animations().update(delta_time);
        global_manager::events().update();
        ////
        ////
        ////

        // debug only !
        must_resolve();
        must_draw();
        // debug only !

        // context::_process_interact();
        _process_resolve();
        // return (has_userspace_thrown() || !global_manager::widgets().drawables.empty());
        return !global_manager::widgets().drawables.empty();
        // return true;
    }

    void process_manager::render()
    {
        draw_overlay([this](ImDrawList* imgui_drawlist) {
            _process_draw(imgui_drawlist);
        });
    }
}
}