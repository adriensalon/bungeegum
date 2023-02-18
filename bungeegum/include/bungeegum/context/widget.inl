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
#include <variant>

namespace bungeegum {
namespace detail {

    template <typename widget_t>
    struct typed_widget_data {
        typed_widget_data(widget_t& widget)
            : _is_external(true)
        {
            _widget = &widget;
        }

        template <typename... args_t>
        typed_widget_data(args_t&&... args)
            : _is_external(false)
        {
            _widget = widget_t(std::forward<args_t>(args)...);
        }

        typed_widget_data(const typed_widget_data& other) = delete;
        typed_widget_data& operator=(const typed_widget_data& other) = delete;

        typed_widget_data(typed_widget_data&& other)
            : _is_external(other._is_external)
        {
            *this = std::move(other);
        }
        typed_widget_data& operator=(typed_widget_data&& other)
        {
            _widget = std::move(other._widget);
            return *this;
        }

        widget_t& operator->()
        {
            if (_is_external)
                return std::get<widget_t*>(_widget);
            return std::get<widget_t&>(_widget);
        }

        // private:
        const bool _is_external;
        std::variant<widget_t, widget_t*> _widget = nullptr;
    };

    // template <typename widget_t, typename... widget_args_t>
    // widget_t& widgets_registry::create(widget_args_t&&... widget_args)
    // {
    //     entity_t _entity = widgets.create_entity();
    //     untyped_widget_data& _widget_data = widgets.create_component<untyped_widget_data>(_entity);
    //     _widget_data.kind = std::make_unique<std::type_index>(typeid(widget_t));
    //     widget_t& _widget = widgets.create_component<widget_t>(_entity, std::forward<widget_args_t>(widget_args)...);
    //     // if (!_widget_data.is_built) {
    //     // widgets.create_component<widget_disabled>(_entity);
    //     // _widget_data.is_built = true;
    //     // }
    //     return _widget;
    // }

    // template <typename widget_t, typename... children_widgets_t>
    // void widgets_registry::build(
    //     widget_t* widget,
    //     children_widgets_t&... children)
    // {
    //     untyped_widget_data& _widget_data = get(*widget);
    //     // _widget_data.is_built = true;
    //     constexpr_foreach<children_widgets_t...>([&](auto& _child_widget) {
    //         untyped_widget_data& _child_widget_data = get(_child_widget);
    //         _child_widget_data.parent = _widget_data;
    //         _widget_data.children.emplace_back(_child_widget_data);
    //     },
    //         children...);
    // }

    template <typename widget_t>
    void widgets_registry::on_resolve(widget_t* widget, const std::function<float2(resolve_command_data&)>& resolver)
    {

        if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(&widget)) == detail::widgets_context.accessors.end())
            register_widget(&widget);
        untyped_widget_data& _widget_data = get(*widget);
        // _widget_data.is_built = true;
        _widget_data.widget_resolver = resolver;
        _widget_data.widget_resolver_data = resolve_command_data(); //????? manque des trucs ici
        must_resolve<widget_t>(*widget);
    }

    template <typename widget_t>
    void widgets_registry::on_draw(widget_t* widget, const std::function<void(const float2, draw_command_data&)>& drawer)
    {

        // if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(&widget)) == detail::widgets_context.accessors.end())
        //     register_widget(&widget);
        untyped_widget_data& _widget_data = get(*widget);
        // _widget_data.is_built = true;
        _widget_data.widget_drawer = drawer;
        _widget_data.widget_drawer_data = draw_command_data();
        must_draw<widget_t>(*widget);
    }

    template <typename widget_t>
    void widgets_registry::build_root(widget_t& widget)
    {

        if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(&widget)) == detail::widgets_context.accessors.end())
            register_widget(&widget);
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
        if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(&widget)) == detail::widgets_context.accessors.end())
            register_widget(&widget);
        untyped_widget_data& _data = get(widget);
        must_draw_heads.emplace_back(std::pair<std::reference_wrapper<untyped_widget_data>, bool> { _data, must_draw_children });
        // if (must_draw_heads.empty())
        //     must_draw_heads.emplace_back(std::pair<std::reference_wrapper<untyped_widget_data>, bool> { _data, must_draw_children });
        // else {
        //     if (must_draw_children) {
        //         unsigned int _index = 0;
        //         std::vector<unsigned int> _remove_indices;
        //         for (const std::pair<std::reference_wrapper<untyped_widget_data>, bool>& _head : must_draw_heads) {
        //             if (is_parent(_data, _head.first.get()))
        //                 _remove_indices.emplace_back(_index);
        //             _index++;
        //         }
        //         for (std::vector<unsigned int>::reverse_iterator _it = _remove_indices.rbegin(); _it != _remove_indices.rend(); _it++) {
        //             must_draw_heads.erase(must_draw_heads.begin() + *_it);
        //         }
        //     }
        //     for (std::pair<std::reference_wrapper<untyped_widget_data>, bool>& _head : must_draw_heads) {
        //         // if (_head.second && !is_parent(_head.first.get(), _data))
        //         must_draw_heads.emplace_back(std::pair<std::reference_wrapper<untyped_widget_data>, bool> { _data, must_draw_children });
        //     }
        // }
    }

    template <typename widget_t>
    untyped_widget_data& widgets_registry::get(widget_t& widget)
    {
        return *(accessors[reinterpret_cast<void*>(&widget)]);
        // return widgets.get_component<untyped_widget_data>(widgets.get_entity(widget));
    }

    // template <typename widget_t>
    // widget_t& widgets_registry::get(untyped_widget_data& data)
    // {
    //     return *(reinterpret_cast<widget_t*>(data.widget));
    // }

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

template <typename widget_t>
widget_t& create_widget()
{
    entity_t _entity = detail::widgets_context.widgets.create_entity();
    detail::widgets_context.widgets.create_component<detail::untyped_widget_data>(_entity);
    widget_t& _w = detail::widgets_context.widgets.create_component<widget_t>(_entity);

    std::cout << detail::widgets_context.widgets.create_component<detail::typed_widget_data<widget_t>>(_entity, _w)._is_external << std::endl;

    if (detail::widgets_context.accessors.find(&_w) == detail::widgets_context.accessors.end())
        detail::widgets_context.inline_widgets.emplace(reinterpret_cast<void*>(&_w), _entity);

    // else {
    //     std::cout << "ICI !! \n";
    //     // move la data vers cette entity et delete ancienne entity avec juste l'ancienne data
    // 	// update accessors
    // }

    std::cout << "creating widget... " << reinterpret_cast<std::uintptr_t>(&_w) << std::endl;
    return _w;
}

template <typename widget_t>
void register_widget(widget_t* widget)
{
    // std::cout << "register ! \n";
    entity_t _entity;
    if (detail::widgets_context.inline_widgets.find(reinterpret_cast<void*>(widget)) != detail::widgets_context.inline_widgets.end()) {
        _entity = detail::widgets_context.inline_widgets.at(reinterpret_cast<void*>(widget));
        detail::widgets_context.inline_widgets.erase(reinterpret_cast<void*>(widget));
        std::cout << "register created ! \n";
        detail::untyped_widget_data& _data = detail::widgets_context.widgets.get_component<detail::untyped_widget_data>(_entity);
        _data.widget = reinterpret_cast<void*>(widget);
        _data.kind = std::make_unique<std::type_index>(typeid(widget_t));
        detail::widgets_context.accessors[_data.widget] = &_data;

        if constexpr (has_draw_function_v<widget_t>)
            detail::widgets_context.on_draw(widget, [widget](const float2& size, detail::draw_command_data& command) { // [=widget] otherwise we pass a reference to ptr
                draw_command _command(command);
                widget->draw(size, _command);
            });
        std::cout << "register created widget [SIMPLE] ... " << reinterpret_cast<std::uintptr_t>(widget) << std::endl;
        return;
    } else {
        // si il a deja un untyped associe on le get
        auto _existing_entity = detail::widgets_context.widgets.try_get_entity(*widget);
        if (_existing_entity != std::nullopt) {
            _entity = _existing_entity.value();
            detail::untyped_widget_data& _data = detail::widgets_context.widgets.get_component<detail::untyped_widget_data>(_entity);
            _data.widget = reinterpret_cast<void*>(widget);
            _data.kind = std::make_unique<std::type_index>(typeid(widget_t));
            detail::widgets_context.accessors[_data.widget] = &_data;

            if constexpr (has_draw_function_v<widget_t>)
                detail::widgets_context.on_draw(widget, [widget](const float2& size, detail::draw_command_data& command) { // [=widget] otherwise we pass a reference to ptr
                    draw_command _command(command);
                    widget->draw(size, _command);
                });
            std::cout << "register created widget [COMPLEX] ... " << reinterpret_cast<std::uintptr_t>(widget) << std::endl;
            return;
        } else {
            // sinon il a pas ete make par nous
            _entity = detail::widgets_context.widgets.create_entity();
        }
    }

    std::cout << detail::widgets_context.widgets.create_component<detail::typed_widget_data<widget_t>>(_entity, *widget)._is_external << std::endl;

    detail::untyped_widget_data& _data = detail::widgets_context.widgets.create_component<detail::untyped_widget_data>(_entity);

    // static std::vector<detail::untyped_widget_data> _datas(10);
    // detail::untyped_widget_data& _data = _datas.emplace_back();

    _data.widget = reinterpret_cast<void*>(widget);
    _data.kind = std::make_unique<std::type_index>(typeid(widget_t));
    detail::widgets_context.accessors[_data.widget] = &_data;

    if constexpr (has_draw_function_v<widget_t>)
        detail::widgets_context.on_draw(widget, [widget](const float2& size, detail::draw_command_data& command) { // [=widget] otherwise we pass a reference to ptr
            draw_command _command(command);
            widget->draw(size, _command);
        });
}

template <typename widget_t>
void unregister_widget(widget_t* widget)
{
}

template <typename widget_t, typename child_widget_t>
void adopt_child_widget(widget_t* widget, child_widget_t& child_widget)
{
    if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(widget)) == detail::widgets_context.accessors.end())
        register_widget(widget);
    if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(&child_widget)) == detail::widgets_context.accessors.end())
        register_widget(&child_widget);
    std::cout << "adopt ! \n";
    detail::untyped_widget_data& _data = detail::widgets_context.get(*widget);
    detail::untyped_widget_data& _child_data = detail::widgets_context.get(child_widget);
    _child_data.parent = _data;
    _data.children.emplace_back(_child_data);
}

template <typename widget_t, typename child_widget_t>
void adopt_children_widgets(widget_t* widget, child_widget_t& children_widgets)
{
    (adopt_child_widget(widget, children_widgets), ...);
}

template <typename widget_t, typename child_widget_t>
void reny_child_widget(widget_t* widget, child_widget_t& child_widget)
{
}

}