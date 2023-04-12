#pragma once

#include <bungeegum/core/exceptions.fwd>

namespace bungeegum {
namespace detail {

    template <typename widget_t>
    untyped_widget_data& get_untyped_widget(widget_t& widget)
    {
        void* _void_widget = reinterpret_cast<void*>(&widget);
        return widgets_context.accessors.at(_void_widget).get();
    }

    template <typename widget_t>
    void register_widget(widget_t* widget)
    {
        void* _void_widget = reinterpret_cast<void*>(widget);
        // std::cout << "register ! \n";
        detail::entity_t _entity;
        if (detail::widgets_context.possessed.find(_void_widget) != detail::widgets_context.possessed.end()) {
            _entity = detail::widgets_context.possessed.at(_void_widget);
            detail::widgets_context.possessed.erase(_void_widget);
            std::cout << "register created ! \n";
            detail::untyped_widget_data& _data = detail::widgets_context.widgets.get_component<detail::untyped_widget_data>(_entity);

            _data.kind = std::make_unique<std::type_index>(typeid(widget_t));
            detail::widgets_context.accessors.insert_or_assign(_void_widget, std::ref(_data));

            detect_on_interact(widget);
            detect_on_resolve(widget);
            detect_on_draw(widget);
            std::cout << "register created widget [SIMPLE] ... " << reinterpret_cast<std::uintptr_t>(widget) << std::endl;
            return;
        } else {
            // si il a deja un untyped associe on le get
            auto _existing_entity = detail::widgets_context.widgets.try_get_entity(*widget);
            if (_existing_entity != std::nullopt) {
                _entity = _existing_entity.value();
                detail::untyped_widget_data& _data = detail::widgets_context.widgets.get_component<detail::untyped_widget_data>(_entity);
                // _data.widget = reinterpret_cast<void*>(widget);
                _data.kind = std::make_unique<std::type_index>(typeid(widget_t));
                detail::widgets_context.accessors.insert_or_assign(_void_widget, std::ref(_data));

                detect_on_interact(widget);
                detect_on_resolve(widget);
                detect_on_draw(widget);
                std::cout << "register created widget [COMPLEX] ... " << reinterpret_cast<std::uintptr_t>(widget) << std::endl;
                return;
            } else {
                // sinon il a pas ete make par nous
                _entity = detail::widgets_context.widgets.create_entity();
            }
        }

        detail::widgets_context.widgets.create_component<std::reference_wrapper<widget_t>>(_entity, *widget);

        detail::untyped_widget_data& _data = detail::widgets_context.widgets.create_component<detail::untyped_widget_data>(_entity);

        _data.kind = std::make_unique<std::type_index>(typeid(widget_t));
        detail::widgets_context.accessors.insert_or_assign(_void_widget, std::ref(_data));

        detect_on_interact(widget);
        detect_on_resolve(widget);
        detect_on_draw(widget);
    }

    template <typename widget_t>
    void unregister_widget(widget_t* widget)
    {
    }

    template <typename widget_t, typename child_widget_t>
    void adopt_widget(widget_t* widget, child_widget_t& child_widget)
    {
        if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(widget)) == detail::widgets_context.accessors.end())
            register_widget(widget);
        if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(&child_widget)) == detail::widgets_context.accessors.end())
            register_widget(&child_widget);
        std::cout << "adopt ! \n";
        detail::untyped_widget_data& _data = get_untyped_widget(*widget);
        detail::untyped_widget_data& _child_data = get_untyped_widget(child_widget);
        _child_data.parent = _data;
        _data.children.emplace_back(_child_data);
    }
}

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    // return detail::widgets_context.create<widget_t>();
    detail::entity_t _entity = detail::widgets_context.widgets.create_entity();
    detail::widgets_context.widgets.create_component<detail::untyped_widget_data>(_entity);
    widget_t& _w = detail::widgets_context.widgets.create_component<widget_t>(_entity, std::forward<widget_args_t>(widget_args)...);

    // std::cout << detail::widgets_context.widgets.create_component<detail::typed_widget_data<widget_t>>(_entity, _w)._is_external << std::endl;
    // detail::widgets_context.widgets.create_component<std::reference_wrapper<widget_t>>(_entity, _w);

    if (detail::widgets_context.accessors.find(&_w) == detail::widgets_context.accessors.end())
        detail::widgets_context.possessed.emplace(reinterpret_cast<void*>(&_w), _entity);

    // else {
    //     std::cout << "ICI !! \n";
    //     // move la data vers cette entity et delete ancienne entity avec juste l'ancienne data
    // 	// update accessors
    // }

    std::cout << "creating widget... " << reinterpret_cast<std::uintptr_t>(&_w) << std::endl;
    return _w;
}

template <typename widget_t>
void unmake(widget_t& widget)
{
    void* _void_widget = reinterpret_cast<void*>(&widget);
    detail::entity_t _entity;
    if (detail::widgets_context.accessors.find(_void_widget) == detail::widgets_context.accessors.end())
        detail::throw_error<detail::error_type::bad_implementation>("widget not found in accessors");
    _entity = detail::widgets_context.possessed.at(_void_widget);
    detail::widgets_context.widgets.destroy_entity(_entity);
    detail::widgets_context.possessed.erase(_void_widget);
    detail::widgets_context.accessors.erase(_void_widget);
}

template <typename widget_t, typename... children_widgets_t>
void adopt(widget_t* widget, children_widgets_t&... children_widgets)
{
    (detail::adopt_widget(widget, children_widgets), ...);
}

template <typename widget_t, typename... children_widgets_t>
void abandon(widget_t* widget, children_widgets_t&... children_widgets)
{
}

template <typename widget_t>
void iterate(const std::function<void(widget_t&)>& iterate_function)
{
    detail::widgets_context.widgets.iterate<std::reference_wrapper<widget_t>>([&iterate_function](std::reference_wrapper<widget_t>& widget) {
        iterate_function(widget.get());
    });
}
}
