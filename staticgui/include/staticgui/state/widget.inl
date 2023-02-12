//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <iostream>

namespace staticgui {
namespace detail {

    template <typename widget_t, typename... widget_args_t>
    widget_t& widget_registry::make(widget_args_t&&... widget_args)
    {
        entity _entity = _registry.create_entity();
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
    void widget_registry::declare(
        widget_t* widget,
        children_widgets_t&... children)
    {
        widget_data& _widget_data = get_data(*widget);
        _widget_data.is_built = true;
        constexpr_foreach<children_widgets_t...>([&](auto& _child_widget) {
            widget_data& _child_widget_data = get_data(_child_widget);
            _child_widget_data.parent = _widget_data;
            _widget_data.children.emplace_back(_child_widget_data);
        },
            children...);
    }

    template <typename widget_t>
    void widget_registry::on_resolve(widget_t* widget, const std::function<simd_array<float, 2>(resolve_command_data&)>& resolver)
    {
        widget_data& _widget_data = get_data(*widget);
        _widget_data.is_built = true;
        _widget_data.resolver = resolver;
        _widget_data.resolve_command = resolve_command_data(); //????? manque des trucs ici
        must_resolve<widget_t>(*widget);
    }

    template <typename widget_t>
    void widget_registry::on_draw(widget_t* widget, const std::function<void(const simd_array<float, 2>&, draw_command_data&)>& drawer)
    {
        widget_data& _widget_data = get_data(*widget);
        _widget_data.is_built = true;
        _widget_data.drawer = drawer;
        _widget_data.command = draw_command_data();
        must_draw<widget_t>(*widget);
    }

    template <typename... children_widgets_t>
    void widget_registry::declare_root(children_widgets_t&... children)
    {
        _roots.clear();
        constexpr_foreach<children_widgets_t...>([&](auto& _child_widget) {
            widget_data& _child_widget_data = get_data(_child_widget);
            _roots.emplace_back(_child_widget_data);
        },
            children...);
    }

    template <typename widget_t>
    void widget_registry::must_resolve(widget_t& widget, const bool must_resolve_children)
    {
        // resolve pareil
        _must_resolve_heads.emplace_back(std::pair<std::reference_wrapper<widget_data>, bool> { get_data(widget), must_resolve_children });
    }

    template <typename widget_t>
    void widget_registry::must_draw(widget_t& widget, const bool must_draw_children)
    {
        widget_data& _data = get_data(widget);
        if (_must_draw_heads.empty())
            _must_draw_heads.emplace_back(std::pair<std::reference_wrapper<widget_data>, bool> { _data, must_draw_children });
        else {
            if (must_draw_children) {
                unsigned int _index = 0;
                std::vector<unsigned int> _remove_indices;
                for (const std::pair<std::reference_wrapper<widget_data>, bool>& _head : _must_draw_heads) {
                    if (is_parent(_data, _head.first.get()))
                        _remove_indices.emplace_back(_index);
                    _index++;
                }
                for (std::vector<unsigned int>::reverse_iterator _it = _remove_indices.rbegin(); _it != _remove_indices.rend(); _it++) {
                    _must_draw_heads.erase(_must_draw_heads.begin() + *_it);
                }
            }
            for (std::pair<std::reference_wrapper<widget_data>, bool>& _head : _must_draw_heads) {
                if (_head.second && !is_parent(_head.first.get(), _data))
                    _must_draw_heads.emplace_back(std::pair<std::reference_wrapper<widget_data>, bool> { _data, must_draw_children });
            }
        }
    }

    template <typename widget_t>
    widget_data& widget_registry::get_data(widget_t& widget)
    {
        entity _entity = _registry.get_entity(widget);
        return _registry.get_component<widget_data>(_entity);
    }

    template <typename widget_t>
    widget_t& widget_registry::get_widget(widget_data& data)
    {
        entity _entity = _registry.get_entity(data);
        return _registry.get_component<widget_t>(_entity);
    }

    template <typename widget_t>
    void widget_registry::iterate_widgets(const std::function<void(widget_t&)>& iterate_callback)
    {
    }

    template <typename... values_t, typename widget_t>
    void widget_registry::detach_to_widget(event_impl<values_t...>& event, widget_t& widget)
    {
        if (!event.is_attached)
            event.rattach_callback();
        event.is_attached = false;
        event.detached_id = generator::create();
        widget_data& _data = get_data(widget);
        _data.detached_events_removers.emplace(event.detached_id, [&]() {
            event.~event_impl<values_t...>(); // destroy event with the widget
        });
        event.rattach_callback = [&]() {
            _data.detached_events_removers.erase(event.detached_id);
        };
    }

    template <typename value_t, typename widget_t>
    void widget_registry::detach_animation(animation_impl<value_t>& animation, widget_t& widget)
    {
        detach_to_widget(animation.event, widget);
    }

    // template <typename parent_widget_t, typename widget_t>
    // std::optional<parent_widget_t&> _get_parent_widget(widget_t& widget)
    // {
    //     widget_data& _widget_data = get_data(widget);
    //     return _widget_data.parent ? *(_widget_data.parent) : std::nullopt;
    // }

    // template <typename widget_t>
    // unsigned int get_depth(widget_t& data)
    // {
    //     return 0;
    // }

}
}