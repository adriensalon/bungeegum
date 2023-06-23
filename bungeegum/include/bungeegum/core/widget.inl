#pragma once

#include <bungeegum/core/access.fwd>
#include <bungeegum/core/exceptions.fwd>

namespace bungeegum {
namespace detail {

    // free

    template <typename widget_t>
    std::uintptr_t get_raw_widget(widget_t& widget)
    {
        return raw_cast(widget);
    }

    inline untyped_widget_data& get_untyped_widget(const std::uintptr_t raw_widget)
    {
        return widgets_context.registered.at(raw_widget).get();
    }

    template <typename widget_t>
    untyped_widget_data& get_untyped_widget(widget_t& widget)
    {
        std::uintptr_t _raw_widget = get_raw_widget(widget);
        return get_untyped_widget(_raw_widget);
    }

    inline bool is_widget_registered(const std::uintptr_t raw_widget)
    {
        return widgets_context.registered.find(raw_widget) != widgets_context.registered.end();
    }

    template <typename widget_t>
    bool is_widget_registered(widget_t& widget)
    {
        std::uintptr_t _raw_widget = get_raw_widget<widget_t>(widget);
        return is_widget_registered(_raw_widget);
    }

    inline bool is_widget_possessed(const std::uintptr_t raw_widget)
    {
        return widgets_context.possessed.find(raw_widget) != widgets_context.possessed.end();
    }

    template <typename widget_t>
    bool is_widget_possessed(widget_t& widget)
    {
        std::uintptr_t _raw_widget = get_raw_widget<widget_t>(widget);
        return is_widget_possessed(_raw_widget);
    }

    template <typename widget_t>
    void assign_widget(widget_t& widget, untyped_widget_data& untyped_widget, const std::uintptr_t raw_widget)
    {
        untyped_widget.raw_widget = raw_widget;
        untyped_widget.kind = std::make_unique<std::type_index>(typeid(widget_t));
        untyped_widget.kindlol = std::string(untyped_widget.kind->name());
        widgets_context.registered.insert_or_assign(raw_widget, std::ref(untyped_widget));
        bungeegum::access::detect_on_interact(widget);
        bungeegum::access::detect_on_resolve(widget);
        bungeegum::access::detect_on_draw(widget);
    }

    template <typename widget_t>
    void register_widget(widget_t& widget, const std::uintptr_t raw_widget)
    {
        entity_t _entity;
        if (is_widget_possessed(raw_widget)) {
            _entity = widgets_context.possessed.at(raw_widget);
            untyped_widget_data& _untyped_widget = widgets_context.widgets.get_component<untyped_widget_data>(_entity);
            assign_widget(widget, _untyped_widget, raw_widget);
            std::cout << "assign 1 \n";
        } else {
            _entity = widgets_context.widgets.create_entity();
            widgets_context.widgets.create_component<std::reference_wrapper<widget_t>>(_entity, widget);
            untyped_widget_data& _untyped_widget = widgets_context.widgets.create_component<untyped_widget_data>(_entity);
            assign_widget(widget, _untyped_widget, raw_widget);
            std::cout << "assign 2 \n";
        }
    }

    template <typename widget_t>
    void unregister_widget(widget_t& widget)
    {
    }

    template <typename widget_t, typename child_widget_t>
    runtime_widget adopt_widget(widget_t& widget, child_widget_t& child_widget)
    {
        std::uintptr_t _raw_widget = get_raw_widget<widget_t>(widget);
        std::uintptr_t _raw_child_widget = get_raw_widget<child_widget_t>(child_widget);
        if (!is_widget_registered(_raw_widget))
            register_widget(widget, _raw_widget);
        if (!is_widget_registered(_raw_child_widget))
            register_widget(child_widget, _raw_child_widget);
        untyped_widget_data& _data = get_untyped_widget(_raw_widget);
        untyped_widget_data& _child_data = get_untyped_widget(_raw_child_widget);
        _child_data.parent = _data;
        _data.children.emplace_back(_child_data);
        return widgets_context.create_adopted(_child_data);
    }

    template <typename widget_t, typename child_widget_t>
    void abandon_widget(widget_t* widget, child_widget_t& child_widget)
    {
        // TODO
    }
}

// runtime_widget

template <typename widget_t>
runtime_widget::runtime_widget(widget_t* widget)
{
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(*widget);
    if (!detail::is_widget_registered(_raw_widget))
        detail::register_widget(*widget, _raw_widget);
    _data.untyped_widget = detail::get_untyped_widget(_raw_widget);
}

template <typename widget_t>
runtime_widget::runtime_widget(widget_t& widget)
{
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(widget);
    if (!detail::is_widget_registered(_raw_widget))
        detail::register_widget(widget, _raw_widget);
    _data.untyped_widget = detail::get_untyped_widget(_raw_widget);
}

// free

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    detail::entity_t _entity = detail::widgets_context.widgets.create_entity();
    detail::widgets_context.widgets.create_component<detail::untyped_widget_data>(_entity);
    widget_t& _widget = detail::widgets_context.widgets.create_component<widget_t>(_entity, std::forward<widget_args_t>(widget_args)...);
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(_widget);
    detail::widgets_context.possessed.emplace(_raw_widget, _entity);

    std::cout << "creating widget... " << reinterpret_cast<std::uintptr_t>(&_widget) << std::endl;
    return _widget;
}

// template <typename widget_t>
// void destroy(widget_t& widget)
// {
// ?
// ?
// ?
// ?
// void* _void_widget = reinterpret_cast<void*>(&widget);
// detail::entity_t _entity;
// if (detail::widgets_context.accessors.find(_void_widget) == detail::widgets_context.accessors.end())
//     detail::throw_error<detail::error_type::bad_implementation>("widget not found in accessors");
// _entity = detail::widgets_context.possessed.at(_void_widget);
// detail::widgets_context.widgets.destroy_entity(_entity);
// detail::widgets_context.possessed.erase(_void_widget);
// detail::widgets_context.accessors.erase(_void_widget);
// }

// template <typename widget_t, typename child_widget_t>
// runtime_widget adopt(widget_t& widget, child_widget_t& child_widget)
// {
//     return detail::adopt_widget(widget, child_widget);
// }

// template <typename widget_t, typename child_widget_t>
// runtime_widget adopt(widget_t* widget, child_widget_t& child_widget)
// {
//     return adopt<widget_t, child_widget_t>(*widget, child_widget);
// }

template <typename widget_t, typename... children_widgets_t>
void abandon(widget_t& widget, children_widgets_t&... children_widgets)
{
    // TODO
    (void)widget;
    ((void)children_widgets, ...);
}

template <typename widget_t, typename... children_widgets_t>
void abandon(widget_t* widget, children_widgets_t&... children_widgets)
{
    abandon<widget_t, children_widgets_t...>(*widget, children_widgets...);
}
}
