#pragma once

#include <bungeegum/core/log.hpp>
#include <bungeegum/core/runtime.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    // widgets_manager

    bool widgets_manager::contains(const std::uintptr_t raw_widget) const
    {
        return _updatables.find(raw_widget) != _updatables.end();
    }

    void widgets_manager::notify_erase(const std::uintptr_t raw_widget)
    {
       _updatables_to_erase.push_back(raw_widget);
    }

    widget_update_data& widgets_manager::operator[](const std::uintptr_t raw_widget)
    {
        return _updatables[raw_widget];
    }

    widget_update_data& widgets_manager::operator[](const runtime_widget& widget)
    {
        return _updatables[raw(widget)];
    }

    std::uintptr_t widgets_manager::raw(const runtime_widget& runtime_widget)
    {
        return runtime_widget._data.raw_widget;
    }

    std::uintptr_t& widgets_manager::root()
    {
        return _root;
    }

    std::uintptr_t widgets_manager::root() const
    {
        return _root;
    }

    widget_update_data& widgets_manager::root_update_data()
    {
        return _updatables[_root];
    }

    // const widget_update_data& widgets_manager::root_update_data() const
    // {
    //     return _updatables.insert_or_assign(_root);
    // }

    runtime_widget widgets_manager::create_runtime_widget(widget_update_data& widget_data)
    {
        return runtime_widget(detail::runtime_widget_data { widget_data.raw_widget });
    }

    void widgets_manager::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback)
    {
        std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
            bool _continue = iterate_callback(_update_data);
            if (_continue) {
                for (auto& _child : _update_data.children) {
                    _iterate(_child.get());
                }
            }
        };
        _iterate(iterate_root);
    }

    void widgets_manager::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback,
        const std::function<void(widget_update_data&)>& tail_iterate_callback)
    {
        std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
            bool _continue = iterate_callback(_update_data);
            if (_continue) {
                for (auto& _child : _update_data.children) {
                    _iterate(_child.get());
                }
            }
            tail_iterate_callback(_update_data);
        };
        _iterate(iterate_root);
    }
}

runtime_widget::runtime_widget(const detail::runtime_widget_data& data)
    : _data(data)
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
    // if (detail::global().widgets->.accessors.find(_void_widget) == detail::global().widgets->.accessors.end())
    //     detail::throw_error<detail::error_type::bad_implementation>("widget not found in accessors");
    // _entity = detail::global().widgets->.possessed.at(_void_widget);
    // detail::global().widgets->.widgets.erase(_entity);
    // detail::global().widgets->.possessed.erase(_void_widget);
    // detail::global().widgets->.accessors.erase(_void_widget);
}

void adopt(const runtime_widget& widget, const runtime_widget& child_widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    detail::widget_update_data& _child_update_data = detail::global().widgets[child_widget];
    _child_update_data.parent = _update_data;
    _update_data.children.emplace_back(_child_update_data);
}

void abandon(const runtime_widget& parent_widget, const runtime_widget& child_widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[parent_widget];
    detail::widget_update_data& _child_update_data = detail::global().widgets[child_widget];
    _child_update_data.parent = std::nullopt;
    for (int1 _k = 0; _k < _update_data.children.size(); _k++) {
        detail::widget_update_data& _loop_child_update_data = _update_data.children[_k].get();
        if (_loop_child_update_data.raw_widget == _child_update_data.raw_widget) {
            _update_data.children.erase(_update_data.children.begin() + _k);
            break;
        }
    }
}

bool has_parent(const runtime_widget& widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    return _update_data.parent.has_value();
}

runtime_widget get_parent(const runtime_widget& widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    if (!_update_data.parent.has_value()) {
        // log_error("Error TODO");
    }
    detail::widget_update_data& _parent_update_data = _update_data.parent.value();
    runtime_widget _parent_widget = detail::global().widgets.create_runtime_widget(_parent_update_data);
    return _parent_widget;
}

resolve_command& get_resolve_command(const runtime_widget& widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    return _update_data.resolver_command;
}

bool has_interact_command(const runtime_widget& widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    return _update_data.interactor_command.has_value();
}

interact_command& get_interact_command(const runtime_widget& widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    if (!_update_data.interactor_command.has_value()) {
        // log_error("Error TODO");
    }
    return _update_data.interactor_command.value();
}

bool has_draw_command(const runtime_widget& widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    return _update_data.drawer_command.has_value();
}

draw_command& get_draw_command(const runtime_widget& widget)
{
    detail::widget_update_data& _update_data = detail::global().widgets[widget];
    if (!_update_data.drawer_command.has_value()) {
        // log_error("Error TODO");
    }
    return _update_data.drawer_command.value();
}
}