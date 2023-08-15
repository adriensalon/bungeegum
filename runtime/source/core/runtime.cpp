#include <bungeegum/core/log.hpp>
#include <bungeegum/core/runtime.hpp>

namespace bungeegum {

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
    // detail::global().widgets->.widgets.destroy_entity(_entity);
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