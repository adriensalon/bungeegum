#pragma once

#include <bungeegum/advanced/interact.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    template <typename widget_t>
    using interact_function = decltype(std::declval<widget_t>().interact(std::declval<interact_command&>()));

    template <typename widget_t>
    constexpr bool has_interact_function_v = detail::is_detected_exact_v<void, interact_function, widget_t>;
}

template <typename widget_t>
void on_interact(widget_t* widget, const std::function<void(interact_command&)>& interact_callback)
{
    if constexpr (detail::has_interact_function_v<widget_t>) {
        detail::untyped_widget_data& _widget_data = detail::widgets_context.get(*widget);
        _widget_data.widget_interactor_command = interact_command();
        _widget_data.widget_interactor = [widget](interact_command& command) { // [=widget] otherwise we pass a reference to ptr
            widget->interact(command);
        };
    }
}

template <typename widget_t>
void must_interact(widget_t* widget)
{
}
}