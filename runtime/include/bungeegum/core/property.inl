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
	detail::widgets_manager& _widgets_manager = detail::global().widgets;
    detail::registry_entity _entity = _widgets_manager[widget].entity;
    detail::value_type_t<property_t>* _property_ptr;
    if constexpr (detail::traits::is_reloadable_v<property_t>) {
        _property_ptr = &(_widgets_manager.widgets.create_component<detail::value_type_t<property_t>>(
            _entity,
            detail::global().backend().reload_manager->allocate<property_t>()));
    } else {
        _property_ptr = &(_widgets_manager.widgets.create_component<detail::value_type_t<property_t>>(_entity));
    }
    property_reference<property_t> _reference = _widgets_manager.create_property_reference<property_t>(*_property_ptr);
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
	detail::widgets_manager& _widgets_manager = detail::global().widgets;
    detail::registry_entity _entity = _widgets_manager[widget].entity;
    return _widgets_manager.widgets.has_component<property_t>(_entity);
}

template <typename property_t>
property_t& get_property(const runtime_widget& widget)
{
	detail::widgets_manager& _widgets_manager = detail::global().widgets;
    detail::registry_entity _entity = _widgets_manager[widget].entity;
    return _widgets_manager.widgets.get_component<property_t>(_entity);
}

template <typename property_t>
property_t& get_or_make_property(const runtime_widget& widget)
{
	if (has_property<property_t>(widget)) {
		return get_property<property_t>(widget);
	}
	return make_property<property_t>(widget);
}

template <typename property_t>
void destroy_property(const runtime_widget& widget)
{
	detail::widgets_manager& _widgets_manager = detail::global().widgets;
    detail::registry_entity _entity = _widgets_manager[widget].entity;
	_widgets_manager.widgets.destroy_component<property_t>(_entity);
}
}