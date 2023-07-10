#include <bungeegum/core/exceptions.hpp>
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
    detail::widget_update_data& _child_untyped_widget = detail::global_widgets_manager.get_widget_data(child_widget);
    detail::protect_userspace([&_child_untyped_widget, &_child_resolve_command]() {
        _child_untyped_widget.resolver(_child_resolve_command);
    });
    return _child_resolve_command._data.resolved_size;
}

void resolve_command::position_child(const runtime_widget& child_widget, const float2 position)
{
    resolve_command& _child_resolve_command = get_resolve_command(child_widget);
    _child_resolve_command._data.resolved_position = position;
}

void must_resolve()
{
    const std::uintptr_t _root = detail::global_widgets_manager.root();
    detail::global_widgets_manager.resolvables.push_back(_root);
}

void must_resolve(const runtime_widget& widget)
{
    const std::uintptr_t _relative_root = detail::global_widgets_manager.raw(widget);
    detail::global_widgets_manager.resolvables.push_back(_relative_root);
}
}