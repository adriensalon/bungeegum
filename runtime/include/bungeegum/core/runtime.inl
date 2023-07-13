#pragma once

#include <bungeegum/backend/backend.fwd>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>

namespace bungeegum {

template <typename widget_t>
runtime_widget::runtime_widget(widget_t* widget)
{
    std::uintptr_t _raw_widget = detail::global().widgets.raw<widget_t>(*widget);
    if (!detail::global().widgets.contains(_raw_widget)) {
        throw detail::backtraced_exception("Errorlol");
    }
    _data.raw_widget = _raw_widget;
}

template <typename widget_t>
runtime_widget::runtime_widget(widget_t& widget)
{
    std::uintptr_t _raw_widget = detail::global().widgets.raw<widget_t>(widget);
    if (!detail::global().widgets.contains(_raw_widget)) {
        //throw
    }
    _data.raw_widget = _raw_widget;
}

template <typename widget_t>
runtime_widget::runtime_widget(widget_reference<widget_t>& widget)
{
    std::uintptr_t _raw_widget = detail::global().widgets.raw<widget_t>(widget.get());
    if (!detail::global().widgets.contains(_raw_widget)) {
        //throw
    }
    _data.raw_widget = _raw_widget;
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
}
