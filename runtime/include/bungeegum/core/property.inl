#pragma once

namespace bungeegum {

namespace detail {

    template <typename property_t>
    property_reference<property_t> widgets_manager::create_property_reference(detail::value_type_t<property_t>& reference)
    {
        return property_reference<property_t>(reference);
    }
}

template <typename property_t>
property_t& property_reference<property_t>::get() const
{
    if constexpr (detail::traits::is_reloadable_v<property_t>) {
        return _data.get();
    } else {
        return _data;
    }
}

template <typename property_t>
property_t* property_reference<property_t>::operator->() const
{
    return std::addressof(get());
}

template <typename property_t>
property_reference<property_t>::property_reference(detail::value_type_t<property_t>& reference)
    : _data(reference)
{
}

template <typename property_t>
property_reference<property_t> make_property_reference(const runtime_widget& widget)
{
    detail::registry_entity _entity = detail::global().widgets[widget].entity;
    detail::value_type_t<property_t>* _widget_ptr;
    if constexpr (detail::traits::is_reloadable_v<property_t>) {
        _widget_ptr = &(detail::global().widgets.widgets.create_component<detail::value_type_t<property_t>>(
            _entity,
            detail::global().backend().reload_manager->allocate<property_t>()));
    } else {
        _widget_ptr = &(detail::global().widgets.widgets.create_component<detail::value_type_t<property_t>>(_entity));
    }
    property_reference<property_t> _reference = detail::global().widgets.create_property_reference<property_t>(*_widget_ptr);
    return _reference;
}

template <typename property_t>
property_t& make_property(const runtime_widget& widget)
{
    return make_property_reference<property_t>(widget).get();
}

template <typename property_t>
bool has_property(const runtime_widget& widget)
{
    (void)widget;
}

template <typename property_t>
property_t& get_property(const runtime_widget& widget)
{
    (void)widget;
}

template <typename property_t>
property_t& get_or_make_property(const runtime_widget& widget)
{
    (void)widget;
}

template <typename property_t>
void destroy_property(const runtime_widget& widget)
{
    (void)widget;
    (void)name;
}
}