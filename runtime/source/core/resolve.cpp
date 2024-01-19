#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/resolve.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {

float2 resolve_command::get_min_size() const
{
    return _data.constraint.min_size;
}

float2 resolve_command::get_max_size() const
{
    return _data.constraint.max_size;
}

void resolve_command::resize(const float2 size)
{
    _data.resolved_size = size;
}

float2 resolve_command::resolve_child(const widget_id& child_id, const float2 min_size, const float2 max_size) const
{
	detail::widget_manager_data& _manager = detail::global().widgets;
	const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    resolve_command& _child_resolve_command = _child_updatable.resolver_command;
    _child_resolve_command._data.constraint.min_size = min_size;
    _child_resolve_command._data.constraint.max_size = max_size;
#if BUNGEEGUM_USE_OVERLAY
    // if (_data.parent_clean_typename.has_value()) {
    //     detail::global().backend.profiler_resolve_chronometer.end_task(_data.parent_clean_typename.value());
    // }
    detail::global().pipelines.profiler_resolve_chronometer.end_task(_data.clean_typename);
    detail::global().pipelines.profiler_resolve_chronometer.begin_task(_child_resolve_command._data.clean_typename);
#endif
    detail::global().logs.protect_userspace([&_child_updatable, &_child_resolve_command]() {
        _child_updatable.resolver(_child_resolve_command);
    });
#if BUNGEEGUM_USE_OVERLAY
    detail::global().pipelines.profiler_resolve_chronometer.end_task(_child_resolve_command._data.clean_typename);
    detail::global().pipelines.profiler_resolve_chronometer.begin_task(_data.clean_typename);
    // if (_data.parent_clean_typename.has_value()) {
    //     detail::global().backend.profiler_resolve_chronometer.begin_task(_data.parent_clean_typename.value());
    // }
#endif
    return _child_resolve_command._data.resolved_size;
}

void resolve_command::position_child(const widget_id& child_id, const float2 position)
{
	detail::widget_manager_data& _manager = detail::global().widgets;
	const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    resolve_command& _child_resolve_command = _child_updatable.resolver_command;
    _child_resolve_command._data.resolved_position = position;
}

void must_resolve(const widget_id& id)
{
	detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
	detail::widget_update_data& _updatable = _manager.updatables[_raw];
    _manager.resolvables.push_back(_updatable);
}
}