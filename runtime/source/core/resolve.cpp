#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/resolve.hpp>
#include <bungeegum/core/runtime.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {

float2 resolve_command::min_size() const
{
    return _data.constraint.min_size;
}

float2 resolve_command::max_size() const
{
    return _data.constraint.max_size;
}

void resolve_command::resize(const float2 size)
{
    _data.resolved_size = size;
}

float2 resolve_command::resolve_child(const runtime_widget& child_widget, const float2 min_size, const float2 max_size) const
{
    std::optional<runtime_widget> _optional_resolving_child;
    if (has_resolve_command(child_widget)) {
		_optional_resolving_child = child_widget;
	} else {
		_optional_resolving_child = get_next_resolving_child(child_widget);
		if (!_optional_resolving_child.has_value()) {
			return _data.resolved_size;
		}
	}	
	const runtime_widget& _resolving_child = _optional_resolving_child.value();
	resolve_command& _child_resolve_command = get_resolve_command(_resolving_child).value();
    _child_resolve_command._data.constraint.min_size = min_size;
    _child_resolve_command._data.constraint.max_size = max_size;
	detail::widget_update_data& _child_update_data = detail::global().widgets[_resolving_child];
#if BUNGEEGUM_USE_OVERLAY
	detail::backend_manager& _backend_manager = detail::global().backend;
	_backend_manager.profiler_resolve_chronometer.end_task(_data.clean_typename);
	_backend_manager.profiler_resolve_chronometer.begin_task(_child_resolve_command._data.clean_typename);
#endif
	detail::global().logs.protect_userspace([&_child_update_data, &_child_resolve_command]() {
		_child_update_data.resolver(_child_resolve_command);
	});
#if BUNGEEGUM_USE_OVERLAY
	_backend_manager.profiler_resolve_chronometer.end_task(_child_resolve_command._data.clean_typename);
	_backend_manager.profiler_resolve_chronometer.begin_task(_data.clean_typename);
#endif
	return _child_resolve_command._data.resolved_size;
}

void resolve_command::position_child(const runtime_widget& child_widget, const float2 position)
{
	std::optional<runtime_widget> _optional_resolving_child;
    if (has_resolve_command(child_widget)) {
		_optional_resolving_child = child_widget;
	} else {
		_optional_resolving_child = get_next_resolving_child(child_widget);
		if (!_optional_resolving_child.has_value()) {
			return;
		}
	}	
	const runtime_widget& _resolving_child = _optional_resolving_child.value();
	resolve_command& _child_resolve_command = get_resolve_command(_resolving_child).value();
	_child_resolve_command._data.resolved_position = position;
}

void must_resolve()
{
	detail::widgets_manager& _widgets_manager = detail::global().widgets;
    const std::uintptr_t _root = _widgets_manager.root();
    _widgets_manager.resolvables.push_back(_root);
}

void must_resolve(const runtime_widget& widget)
{
	detail::widgets_manager& _widgets_manager = detail::global().widgets;
    const std::uintptr_t _relative_root = _widgets_manager.raw(widget);
    _widgets_manager.resolvables.push_back(_relative_root);
}
}