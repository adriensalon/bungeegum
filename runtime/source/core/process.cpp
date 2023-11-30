#include <atomic>
#include <execution>

#include <bungeegum/backend/backend.fwd>
#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/event.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.fwd>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/process.fwd>
#include <bungeegum/core/runtime.hpp>

namespace bungeegum {
namespace detail {

    void process_manager::_process_interact()
    {
        detail::global().backend.profiler_frame_chronometer.begin_task("interact");
#define traverse_interact_impl(interaction_name)                                                                           \
    for (const interaction_name##_event& _event : interaction_name##_events) {                                             \
        global_widgets_manager.traverse(global_widgets_manager.root.value(), [&_event](widget_update_data& _widget_data) { \
            if (_widget_data.interactor_command.has_value()) {                                                             \
                _widget_data.interactor_command.value()._data.is_blocked = false;                                          \
                _widget_data.interactor_command.value()._data.command_data = _event;                                       \
                global().logs.protect_userspace([&_widget_data]() {                                                        \
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
        detail::global().backend.profiler_frame_chronometer.end_task("interact");
    }

    void process_manager::_process_resolve()
    {
        backend_manager& _backend_manager = global().backend;
        widgets_manager& _widgets_manager = global().widgets;
        _backend_manager.profiler_frame_chronometer.begin_task("resolve widgets");
        std::atomic<bool> _resolve_done(false);
        while (!_resolve_done.load()) {
            std::for_each(
                std::execution::seq, // go parallel
                _widgets_manager.resolvables.begin(),
                _widgets_manager.resolvables.end(),
                [&_backend_manager, &_widgets_manager](const std::uintptr_t _raw_widget) {
                    runtime_widget _widget = _widgets_manager.create_runtime_widget(_raw_widget);
                    std::optional<runtime_widget> _optional_resolving;
                    if (has_resolve_command(_widget)) {
                        _optional_resolving = _widget;
                    } else {
                        _optional_resolving = get_next_resolving_child(_widget);
                        if (!_optional_resolving.has_value()) {
                            return;
                        }
                    }
                    const runtime_widget& _resolving = _optional_resolving.value();
					widget_update_data& _update_data = _widgets_manager[_resolving];
                    resolve_command& _resolve_command = _update_data.resolver_command.value();
                    if (_raw_widget == _widgets_manager.root()) {
                        _resolve_command._data.constraint.min_size = _backend_manager.viewport_size;
                        _resolve_command._data.constraint.max_size = _backend_manager.viewport_size;
                    } else {
						std::optional<runtime_widget> _optional_resolving_parent = get_previous_resolving_parent(_widget);
						if (!_optional_resolving_parent.has_value()) {
							return;
						}
						const runtime_widget& _resolving_parent = _optional_resolving_parent.value();
						resolve_command& _parent_resolve_command = get_resolve_command(_resolving_parent).value();
						_resolve_command._data.constraint.min_size = _parent_resolve_command.min_size();
                        _resolve_command._data.constraint.max_size = _parent_resolve_command.max_size();

                    }
#if BUNGEEGUM_USE_OVERLAY
                    _backend_manager.profiler_resolve_chronometer.begin_task(_update_data.resolver_command.value()._data.clean_typename);
#endif
                    global().logs.protect_userspace([&_update_data]() {
                        _update_data.resolver(_update_data.resolver_command.value());
                    });
#if BUNGEEGUM_USE_OVERLAY
                    _backend_manager.profiler_resolve_chronometer.end_task(_update_data.resolver_command.value()._data.clean_typename);
#endif
                });
            _widgets_manager.resolvables.erase(
                _widgets_manager.resolvables.begin(),
                _widgets_manager.resolvables.end());
            _resolve_done.store(_widgets_manager.resolvables.empty());
        }
        _backend_manager.profiler_frame_chronometer.end_task("resolve widgets");
    }

    void process_manager::_process_draw(ImDrawList* imgui_drawlist)
    {
        bool _draw_done = false;

        while (!_draw_done) {
            std::for_each(
                std::execution::seq, // go parallel
                global().widgets.drawables.begin(),
                global().widgets.drawables.end(),
                [imgui_drawlist](const std::uintptr_t _raw_widget) {
                    widget_update_data& _widget_data = global().widgets[_raw_widget];
                    global().widgets.traverse(_widget_data, [imgui_drawlist](widget_update_data& _widget_data) {
                        // accumulate position if 
                    	runtime_widget _widget = global().widgets.create_runtime_widget(_widget_data);
						std::optional<runtime_widget> _optional_resolving;
						if (has_resolve_command(_widget)) {
							_optional_resolving = _widget;
						} else {
							_optional_resolving = get_next_resolving_child(_widget);
							if (!_optional_resolving.has_value()) {
								return true;
							}
						}
                        resolve_command& _widget_resolver_command = get_resolve_command(_optional_resolving.value()).value();
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
#if BUNGEEGUM_USE_OVERLAY
                            _widget_drawer_command._data.commands_infos.clear();
                            std::string _clean_typename = clean_typename(_widget_data.kind->name());
                            global().backend.profiler_draw_chronometer.begin_task(_clean_typename);
#endif
                            global().logs.protect_userspace([&_widget_data, &_widget_drawer_command]() {
                                _widget_data.drawer(_widget_drawer_command);
                            });
#if BUNGEEGUM_USE_OVERLAY
                            global().backend.profiler_draw_chronometer.end_task(_clean_typename);
#endif
                            _widget_drawer_command._data.draw(imgui_drawlist);
                        }
                        return true;
                    });
                });
            global().widgets.drawables.erase(
                global().widgets.drawables.begin(),
                global().widgets.drawables.end());
            _draw_done = global().widgets.drawables.empty();
        }
    }

    bool process_manager::update(const std::chrono::milliseconds& delta_time)
    {
        detail::global().backend.profiler_frame_chronometer.begin_task("animations");
        global().animations.update(delta_time);
        // std::this_thread::sleep_for(std::chrono::milliseconds(2));
        detail::global().backend.profiler_frame_chronometer.end_task("animations");
        detail::global().backend.profiler_frame_chronometer.begin_task("events");
        global().events.update();
        // std::this_thread::sleep_for(std::chrono::milliseconds(2));
        detail::global().backend.profiler_frame_chronometer.end_task("events");
        ////
        ////
        ////

        // debug only !
        must_resolve();
        must_draw();
        // debug only !

        // context::_process_interact();
        _process_resolve();
        // return (has_userspace_thrown() || !global().widgets.drawables.empty());

#if BUNGEEGUM_USE_OVERLAY
        global().backend.lifetime_duration += delta_time;
#endif

        return !global().widgets.drawables.empty();
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