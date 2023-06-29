#pragma once

#include <bungeegum/backend/common.fwd>
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
    void assign_widget(widget_t& widget, untyped_widget_data& untyped_widget, const std::uintptr_t raw_widget, const entity_t entity)
    {
        untyped_widget.raw_widget = raw_widget;
        untyped_widget.kind = std::make_unique<std::type_index>(typeid(widget_t));
        untyped_widget.kind_debug = std::string(untyped_widget.kind->name());
        widgets_context.registered.insert_or_assign(raw_widget, std::ref(untyped_widget));
        bungeegum::access::detect_on_interact(widget);
        bungeegum::access::detect_on_resolve(widget, entity);
        bungeegum::access::detect_on_draw(widget, entity);
    }

    template <typename widget_t>
    void register_widget(widget_t& widget, const std::uintptr_t raw_widget)
    {
        entity_t _entity;
        if (is_widget_possessed(raw_widget)) {
            _entity = widgets_context.possessed.at(raw_widget);
            untyped_widget_data& _untyped_widget = widgets_context.widgets.get_component<untyped_widget_data>(_entity);
            assign_widget(widget, _untyped_widget, raw_widget, _entity);
            // std::cout << "assign 1 \n";
        } else {
            _entity = widgets_context.widgets.create_entity();
            widgets_context.widgets.create_component<std::reference_wrapper<widget_t>>(_entity, widget);
            untyped_widget_data& _untyped_widget = widgets_context.widgets.create_component<untyped_widget_data>(_entity);
            assign_widget(widget, _untyped_widget, raw_widget, _entity);
            // std::cout << "assign 2 \n";
        }
    }
}

// reference_widget

template <typename widget_t>
widget_t& reference_widget<widget_t>::operator&()
{
    return _data.get();
}

template <typename widget_t>
const widget_t& reference_widget<widget_t>::operator&() const
{
    return _data.get();
}

template <typename widget_t>
reference_widget<widget_t>::reference_widget(detail::reloaded<widget_t>&& reloaded)
    : _data(std::move(reloaded))
{
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
    if (!detail::is_widget_registered(_raw_widget)) {
#if 1
        detail::register_widget(widget, _raw_widget);
#else
        detail::register_widget(widget.get(), _raw_widget);
#endif
    }
    _data.untyped_widget = detail::get_untyped_widget(_raw_widget);
}

// free

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    detail::entity_t _entity = detail::widgets_context.widgets.create_entity();
    detail::widgets_context.widgets.create_component<detail::untyped_widget_data>(_entity);

#if 0
	widget_t& _widget = detail::widgets_context.widgets.create_component<widget_t>(_entity, std::forward<widget_args_t>(widget_args)...);
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(_widget);
#else
    detail::reloaded<widget_t>& _widget_reference = detail::widgets_context.widgets.create_component<detail::reloaded<widget_t>>(_entity);
    // copy or move from args
    _widget_reference = detail::reload_manager->allocate<widget_t>();
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(_widget_reference.get());
#endif

    detail::widgets_context.possessed.emplace(_raw_widget, _entity);

    // std::cout << "creating widget... " << reinterpret_cast<std::uintptr_t>(&_widget_reference) << std::endl;
    return _widget_reference.get();
}

template <template <typename, typename> typename container_t, typename allocator_t>
void get_children(
    const runtime_widget& widget,
    container_t<runtime_widget, allocator_t>& container)
{
    (void)widget;
    (void)container;
}

template <typename widget_t>
bool has_type(const runtime_widget& widget)
{
    (void)widget;
    return false;
}

template <typename property_t>
property_t& make_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t>
bool has_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t>
property_t& get_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t>
property_t& get_or_make_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t, template <typename, typename> typename container_t, typename allocator_t>
void get_children_with_property(
    const runtime_widget& widget,
    const std::string& name,
    container_t<runtime_widget, allocator_t>& container,
    const bool recursive)
{
    (void)widget;
    (void)name;
    (void)container;
    (void)recursive;
}

template <typename property_t>
bool has_parent_with_property(const runtime_widget& widget, const std::string& name,
    const unsigned int depth)
{
    (void)depth;
}

template <typename property_t>
runtime_widget get_parent_with_property(
    const runtime_widget& widget,
    const std::string& name,
    const unsigned int depth)
{
    (void)widget;
    (void)name;
    (void)depth;
}

template <typename property_t>
void destroy_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}
}
