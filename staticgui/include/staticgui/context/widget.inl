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
    widget_t& widgets_registry::create(widget_args_t&&... widget_args)
    {
        entity_t _entity = widgets.create_entity();
        untyped_widget_data& _widget_data = widgets.create_component<untyped_widget_data>(_entity);
        _widget_data.kind = std::make_unique<std::type_index>(typeid(widget_t));
        widget_t& _widget = widgets.create_component<widget_t>(_entity, std::forward<widget_args_t>(widget_args)...);
        // if (!_widget_data.is_built) {
        // widgets.create_component<widget_disabled>(_entity);
        // _widget_data.is_built = true;
        // }
        return _widget;
    }

    template <typename widget_t, typename... children_widgets_t>
    void widgets_registry::build(
        widget_t* widget,
        children_widgets_t&... children)
    {
        untyped_widget_data& _widget_data = get(*widget);
        // _widget_data.is_built = true;
        constexpr_foreach<children_widgets_t...>([&](auto& _child_widget) {
            untyped_widget_data& _child_widget_data = get(_child_widget);
            _child_widget_data.parent = _widget_data;
            _widget_data.children.emplace_back(_child_widget_data);
        },
            children...);
    }

    template <typename widget_t>
    void widgets_registry::on_resolve(widget_t* widget, const std::function<float2(resolve_command_data&)>& resolver)
    {
        untyped_widget_data& _widget_data = get(*widget);
        // _widget_data.is_built = true;
        _widget_data.widget_resolver = resolver;
        _widget_data.widget_resolver_data = resolve_command_data(); //????? manque des trucs ici
        must_resolve<widget_t>(*widget);
    }

    template <typename widget_t>
    void widgets_registry::on_draw(widget_t* widget, const std::function<void(const float2, draw_command_data&)>& drawer)
    {
        untyped_widget_data& _widget_data = get(*widget);
        // _widget_data.is_built = true;
        _widget_data.widget_drawer = drawer;
        _widget_data.widget_drawer_data = draw_command_data();
        must_draw<widget_t>(*widget);
    }

    template <typename widget_t>
    void widgets_registry::build_root(widget_t& widget)
    {
        root = get(widget);
    }

    template <typename widget_t>
    void widgets_registry::must_resolve(widget_t& widget, const bool must_resolve_children)
    {
        // resolve pareil
        must_resolve_heads.emplace_back(std::pair<std::reference_wrapper<untyped_widget_data>, bool> { get(widget), must_resolve_children });
    }

    template <typename widget_t>
    void widgets_registry::must_draw(widget_t& widget, const bool must_draw_children)
    {
        untyped_widget_data& _data = get(widget);
        if (must_draw_heads.empty())
            must_draw_heads.emplace_back(std::pair<std::reference_wrapper<untyped_widget_data>, bool> { _data, must_draw_children });
        else {
            if (must_draw_children) {
                unsigned int _index = 0;
                std::vector<unsigned int> _remove_indices;
                for (const std::pair<std::reference_wrapper<untyped_widget_data>, bool>& _head : must_draw_heads) {
                    if (is_parent(_data, _head.first.get()))
                        _remove_indices.emplace_back(_index);
                    _index++;
                }
                for (std::vector<unsigned int>::reverse_iterator _it = _remove_indices.rbegin(); _it != _remove_indices.rend(); _it++) {
                    must_draw_heads.erase(must_draw_heads.begin() + *_it);
                }
            }
            for (std::pair<std::reference_wrapper<untyped_widget_data>, bool>& _head : must_draw_heads) {
                if (_head.second && !is_parent(_head.first.get(), _data))
                    must_draw_heads.emplace_back(std::pair<std::reference_wrapper<untyped_widget_data>, bool> { _data, must_draw_children });
            }
        }
    }

    template <typename widget_t>
    untyped_widget_data& widgets_registry::get(widget_t& widget)
    {
        entity_t _entity = widgets.get_entity(widget);
        return widgets.get_component<untyped_widget_data>(_entity);
    }

    template <typename widget_t>
    widget_t& widgets_registry::get(untyped_widget_data& data)
    {
        entity_t _entity = widgets.get_entity(data);
        return widgets.get_component<widget_t>(_entity);
    }

    template <typename widget_t>
    void widgets_registry::iterate(const std::function<void(widget_t&)>& iterate_callback)
    {
    }

    template <typename... values_t, typename widget_t>
    void widgets_registry::detach(typed_event_data<values_t...>& event, widget_t& widget)
    {
        if (!event.is_attached)
            event.rattach_callback();
        event.is_attached = false;
        event.detached_id = generator::create();
        untyped_widget_data& _data = get_data(widget);
        _data.detached_events_removers.emplace(event.detached_id, [&]() {
            event.~event_impl<values_t...>(); // destroy event with the widget
        });
        event.rattach_callback = [&]() {
            _data.detached_events_removers.erase(event.detached_id);
        };
    }

    template <typename value_t, typename widget_t>
    void widgets_registry::detach(typed_animation_data<value_t>& animation, widget_t& widget)
    {
        detach(animation.event, widget);
    }

    // template <typename parent_widget_t, typename widget_t>
    // std::optional<parent_widget_t&> _get_parent_widget(widget_t& widget)
    // {
    //     untyped_widget_data& _widget_data = get_data(widget);
    //     return _widget_data.parent ? *(_widget_data.parent) : std::nullopt;
    // }

    // template <typename widget_t>
    // unsigned int get_depth(widget_t& data)
    // {
    //     return 0;
    // }

}

template <typename widget_t, typename... widget_args_t>
widget_t& create(widget_args_t&&... widget_args)
{
    return detail::widgets_context.create<widget_t>(std::forward<widget_args_t>(widget_args)...);
}

template <typename widget_t, typename... children_widgets_t>
void build(widget_t* widget, children_widgets_t&... children_widgets)
{
    detail::widgets_context.build(widget, children_widgets...);
    if constexpr (has_resolve_function_v<widget_t, children_widgets_t...>)
        detail::widgets_context.on_resolve<widget_t>(widget, [widget, &children_widgets...](const detail::resolve_command_data& command) {
            resolve_command _command(command);
            return widget->resolve(_command, children_widgets...);
        });
    else
        std::cout << "WARNING : type " << typeid(widget_t).name() << " does not declare a resolve method!" << std::endl;
    if constexpr (has_draw_function_v<widget_t>)
        detail::widgets_context.on_draw(widget, [widget](const float2& size, detail::draw_command_data& command) { // [=widget] otherwise we pass a reference to ptr
            draw_command _command(command);
            widget->draw(size, _command);
        });
    else
        std::cout << "WARNING : type " << typeid(widget_t).name() << " does not declare a draw method!" << std::endl;
}

}