//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {
namespace detail {

    template <typename widget_t, typename... widget_args_t>
    widget_t& widgets_registry::make(widget_args_t&&... widget_args)
    {
        glue::id_integer _entity = _registry.create_entity();
        widget_data& _widget_data = _registry.create_component<widget_data>(_entity);
        _widget_data.kind = std::make_unique<std::type_index>(typeid(widget_t));
        widget_t& _widget = _registry.create_component<widget_t>(_entity, std::forward<widget_args_t>(widget_args)...);
        if (!_widget_data.is_built) {
            _registry.create_component<widget_disabled>(_entity);
            _widget_data.is_built = true;
        }
        return _widget;
    }

    template <typename widget_t, typename... children_widgets_t>
    void widgets_registry::build(
        widget_t* widget,
        std::function<void(layout_state&)> paint_callback,
        children_widgets_t&... children)
    {
        widget_data& _widget_data = get_data(*widget);
        _widget_data.is_built = true;
        _widget_data.paint_callback = paint_callback;
        glue::constexpr_foreach<children_widgets_t...>([&](auto& _child_widget) {
            widget_data& _child_widget_data = get_data(_child_widget);
            _child_widget_data.parent = _widget_data;
            _widget_data.children.emplace_back(_child_widget_data);
        },
            children...);
    }

    template <typename... children_widgets_t>
    void widgets_registry::build_roots(children_widgets_t&... children)
    {
        _roots.clear();
        glue::constexpr_foreach<children_widgets_t...>([&](auto& _child_widget) {
            widget_data& _child_widget_data = get_data(_child_widget);
            _roots.emplace_back(_child_widget_data);
        },
            children...);
    }

    template <typename widget_t>
    widget_data& widgets_registry::get_data(widget_t& widget)
    {
        glue::id_integer _entity = _registry.get_entity(widget);
        return _registry.get_component<widget_data>(_entity);
    }

    template <typename widget_t>
    widget_t& widgets_registry::get_widget(widget_data& data)
    {
        glue::id_integer _entity = _registry.get_entity(data);
        return _registry.get_component<widget_t>(_entity);
    }

    template <typename widget_t, typename parent_widget_t>
    std::optional<parent_widget_t&> get_parent_widget(widget_t& widget)
    {
        widget_data& _widget_data = get_data(widget);
        return _widget_data.parent ? *(_widget_data.parent) : std::nullopt;
    }

    template <typename widget_t>
    unsigned int get_depth(widget_t& data)
    {
        return 0;
    }

    template <typename widget_t>
    void widgets_registry::iterate_widgets(const std::function<void(widget_t&)>& iterate_callback)
    {
    }

}
}