#pragma once

#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    // widgets_registry

    runtime_widget widgets_registry::get_runtime_widget(untyped_widget_data& widget_data)
    {
        runtime_widget _runtime_widget;
        _runtime_widget._data.untyped_widget = widget_data;
        return _runtime_widget;
    }

    untyped_widget_data& widgets_registry::get_widget_data(const runtime_widget& widget)
    {
        return widget._data.untyped_widget.value().get();
    }

    void widgets_registry::traverse_untyped(untyped_widget_data& iterate_root, const std::function<bool(untyped_widget_data&)>& iterate_callback)
    {
        std::function<void(untyped_widget_data&)> _iterate = [&](untyped_widget_data& _widget_data) {
            bool _continue = iterate_callback(_widget_data);
            if (_continue)
                for (auto& _child : _widget_data.children)
                    _iterate(_child.get());
        };
        _iterate(iterate_root);
    }
}

runtime_widget::runtime_widget()
{
}

void destroy(const runtime_widget& widget)
{
    (void)widget;
    // ?
    // ?
    // ?
    // ?
    // void* _void_widget = reinterpret_cast<void*>(&widget);
    // detail::registry_entity _entity;
    // if (detail::widgets_context.accessors.find(_void_widget) == detail::widgets_context.accessors.end())
    //     detail::throw_error<detail::error_type::bad_implementation>("widget not found in accessors");
    // _entity = detail::widgets_context.possessed.at(_void_widget);
    // detail::widgets_context.widgets.destroy_entity(_entity);
    // detail::widgets_context.possessed.erase(_void_widget);
    // detail::widgets_context.accessors.erase(_void_widget);
}

void adopt(const runtime_widget& widget, const runtime_widget& child_widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    detail::untyped_widget_data& _child_widget_data = detail::widgets_context.get_widget_data(child_widget);
    _child_widget_data.parent = _widget_data;
    _widget_data.children.emplace_back(_child_widget_data);
}

void abandon(const runtime_widget& parent_widget, const runtime_widget& child_widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(parent_widget);
    detail::untyped_widget_data& _child_widget_data = detail::widgets_context.get_widget_data(child_widget);
    _child_widget_data.parent = std::nullopt;
    for (int1 _k = 0; _k < _widget_data.children.size(); _k++) {
        detail::untyped_widget_data& _loop_child_widget_data = _widget_data.children[_k].get();
        if (_loop_child_widget_data == _child_widget_data) {
            _widget_data.children.erase(_widget_data.children.begin() + _k);
            break;
        }
    }
}

bool has_parent(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    return _widget_data.parent.has_value();
}

runtime_widget get_parent(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    if (!_widget_data.parent.has_value()) {
        throw_error("Error TODO");
    }
    detail::untyped_widget_data& _parent_widget_data = _widget_data.parent.value();
    runtime_widget _parent_widget = detail::widgets_context.get_runtime_widget(_parent_widget_data);
    return _parent_widget;
}

resolve_command& get_resolve_command(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    return _widget_data.resolver_command.value();
}

bool has_interact_command(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    return _widget_data.interactor_command.has_value();
}

interact_command& get_interact_command(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    if (!_widget_data.interactor_command.has_value()) {
        throw_error("Error TODO");
    }
    return _widget_data.interactor_command.value();
}

bool has_draw_command(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    return _widget_data.drawer_command.has_value();
}

draw_command& get_draw_command(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get_widget_data(widget);
    if (!_widget_data.drawer_command.has_value()) {
        throw_error("Error TODO");
    }
    return _widget_data.drawer_command.value();
}
}