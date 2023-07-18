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
    resolve_command& _child_resolve_command = get_resolve_command(child_widget);
    _child_resolve_command._data.constraint.min_size = min_size;
    _child_resolve_command._data.constraint.max_size = max_size;
    detail::widget_update_data& _child_untyped_widget = detail::global().widgets[child_widget];
#if BUNGEEGUM_USE_OVERLAY
    // if (_data.parent_clean_typename.has_value()) {
    //     detail::global().backend.profiler_resolve_chronometer.end_task(_data.parent_clean_typename.value());
    // }
    detail::global().backend.profiler_resolve_chronometer.end_task(_data.clean_typename);
    detail::global().backend.profiler_resolve_chronometer.begin_task(_child_resolve_command._data.clean_typename);
#endif
    detail::global().logs.protect_userspace([&_child_untyped_widget, &_child_resolve_command]() {
        _child_untyped_widget.resolver(_child_resolve_command);
    });
#if BUNGEEGUM_USE_OVERLAY
    detail::global().backend.profiler_resolve_chronometer.end_task(_child_resolve_command._data.clean_typename);
    detail::global().backend.profiler_resolve_chronometer.begin_task(_data.clean_typename);
    // if (_data.parent_clean_typename.has_value()) {
    //     detail::global().backend.profiler_resolve_chronometer.begin_task(_data.parent_clean_typename.value());
    // }
#endif
    return _child_resolve_command._data.resolved_size;
}

void resolve_command::position_child(const runtime_widget& child_widget, const float2 position)
{
    resolve_command& _child_resolve_command = get_resolve_command(child_widget);
    _child_resolve_command._data.resolved_position = position;
}

void must_resolve()
{
    const std::uintptr_t _root = detail::global().widgets.root();
    detail::global().widgets.resolvables.push_back(_root);
}

void must_resolve(const runtime_widget& widget)
{
    const std::uintptr_t _relative_root = detail::global().widgets.raw(widget);
    detail::global().widgets.resolvables.push_back(_relative_root);
}
}