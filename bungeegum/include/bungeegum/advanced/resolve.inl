#pragma once

#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    template <typename widget_t, typename... children_widgets_t>
    using resolve_function = decltype(std::declval<widget_t>().resolve<children_widgets_t...>(std::declval<resolve_command&>(), std::declval<children_widgets_t&>()...));

    template <typename widget_t, typename... children_widgets_t>
    constexpr bool has_resolve_function_v = is_detected_exact_v<float2, resolve_function, widget_t, children_widgets_t...>;
}

template <typename widget_t, typename... children_widgets_t>
void on_resolve(
    widget_t* widget,
    const std::function<float2(resolve_command&, children_widgets_t&...)>& resolve_callback,
    children_widgets_t&... children_widgets)
{
    // todo
}

template <typename widget_t>
void must_resolve(widget_t* widget)
{
    // todo
}

}