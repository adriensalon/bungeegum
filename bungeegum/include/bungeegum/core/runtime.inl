#pragma once

#include <bungeegum/backend/common.fwd>
#include <bungeegum/core/exceptions.fwd>

namespace bungeegum {

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
}
