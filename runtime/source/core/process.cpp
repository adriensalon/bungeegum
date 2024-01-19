#include <execution>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/event.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/hotswap.fwd>
#include <bungeegum/core/log.fwd>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/process.fwd>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    void process_manager::_process_interact()
    {
        detail::global().pipelines.profiler_frame_chronometer.begin_task("interact");
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
        detail::global().pipelines.profiler_frame_chronometer.end_task("interact");
    }

    void process_manager::_process_resolve(const float2 viewport_size, widget_update_data& root_updatable)
    {
    	logs_manager& _log_manager = global().logs;
    	pipelines_manager& _pipeline_manager = global().pipelines;
    	widget_manager_data& _widget_manager = global().widgets;
#if BUNGEEGUM_USE_OVERLAY
        _pipeline_manager.profiler_frame_chronometer.begin_task("resolve widgets");
#endif
        bool _resolve_done = false;
        while (!_resolve_done) {
			std::size_t _iteration_size = _widget_manager.resolvables.size();
            for (widget_update_data& _updatable : _widget_manager.resolvables)
            {
                resolve_command& _resolve_command = _updatable.resolver_command;
                if (_updatable.raw == root_updatable.raw) {
                    _resolve_command._data.constraint.min_size = viewport_size;
                    _resolve_command._data.constraint.max_size = viewport_size;
                } else {
                    widget_update_data& _parent_widget_data_ptr = _updatable.parent.value().get();
                    resolve_command& _parent_resolve_command = _parent_widget_data_ptr.resolver_command;
                    _resolve_command._data.constraint.min_size = _parent_resolve_command.get_min_size();
                    _resolve_command._data.constraint.max_size = _parent_resolve_command.get_max_size();
                }
#if BUNGEEGUM_USE_OVERLAY
                _pipeline_manager.profiler_resolve_chronometer.begin_task(_resolve_command._data.clean_typename);
#endif
                _log_manager.protect_userspace([&_updatable, &_resolve_command]() {
                    _updatable.resolver(_resolve_command);
                });
#if BUNGEEGUM_USE_OVERLAY
                _pipeline_manager.profiler_resolve_chronometer.end_task(_resolve_command._data.clean_typename);
#endif
            };
            _widget_manager.resolvables.erase(
                _widget_manager.resolvables.begin(),
                _widget_manager.resolvables.begin() + _iteration_size);
            _resolve_done = _widget_manager.resolvables.empty();
        }
#if BUNGEEGUM_USE_OVERLAY
        _pipeline_manager.profiler_frame_chronometer.end_task("resolve widgets");
#endif
    }

    void process_manager::_process_draw(ImDrawList* imgui_drawlist)
    {
		global_manager& _global = global();
        bool _draw_done = false;

        while (!_draw_done) {
            for (widget_update_data& _updatable : _global.widgets.drawables) { // go default to draw_children()
				resolve_command& _widget_resolver_command = _updatable.resolver_command;
                    if (_updatable.parent.has_value()) {
                        resolve_command& _parent_resolver_command = _updatable.parent.value().get().resolver_command;
                        _widget_resolver_command._data.accumulated_position = _widget_resolver_command._data.resolved_position + _parent_resolver_command._data.accumulated_position;
                    }
					draw_command& _widget_drawer_command = _updatable.drawer_command;
					_widget_drawer_command._data.resolved_size = _widget_resolver_command._data.resolved_size;
					_widget_drawer_command._data.resolved_position = _widget_resolver_command._data.accumulated_position;
					_widget_drawer_command._data.commands.clear();
// #if BUNGEEGUM_USE_OVERLAY
//                         _widget_drawer_command._data.commands_infos.clear();
//                         std::string _clean_typename = global().pipelines.to_clean_typename(_updatable.inplace_data.type().name());
//                         global().pipelines.profiler_draw_chronometer.begin_task(_clean_typename);
// #endif
					_global.logs.protect_userspace([&_updatable, &_widget_drawer_command]() {
						_updatable.drawer(_widget_drawer_command);
					});
// #if BUNGEEGUM_USE_OVERLAY
//                         global().pipelines.profiler_draw_chronometer.end_task(_clean_typename);
// #endif
					_widget_drawer_command._data.draw(imgui_drawlist);
            };
            _global.widgets.drawables.erase(_global.widgets.drawables.begin(), _global.widgets.drawables.end());
            _draw_done = _global.widgets.drawables.empty();
        }
    }

    bool process_manager::update(const float2 viewport_size, const std::chrono::milliseconds& delta_time, widget_update_data& root_updatable)
    {
        detail::global().pipelines.profiler_frame_chronometer.begin_task("animations");
		// std::cout << "before animations\n";
        global().animations.update(delta_time);
        // std::this_thread::sleep_for(std::chrono::milliseconds(2));
        detail::global().pipelines.profiler_frame_chronometer.end_task("animations");
        detail::global().pipelines.profiler_frame_chronometer.begin_task("events");
		// std::cout << "before events\n";
        global().events.update();
        // std::this_thread::sleep_for(std::chrono::milliseconds(2));
        detail::global().pipelines.profiler_frame_chronometer.end_task("events");
        ////
        ////
        ////

        // debug only !
        // must_resolve();
        // must_draw();
        // debug only !

        // context::_process_interact();
		// std::cout << "before pr\n";
        // _process_resolve(root_updatable);
		// std::cout << "after pr\n";
        // return (has_userspace_thrown() || !global().widgets.drawables.empty());

#if BUNGEEGUM_USE_OVERLAY
        global().pipelines.lifetime_duration += delta_time;
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