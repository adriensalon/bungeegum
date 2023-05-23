#pragma once

#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    template <typename widget_t>
    using draw_function = decltype(std::declval<widget_t>().draw(std::declval<draw_command&>()));

    template <typename widget_t>
    constexpr bool has_draw_function = is_detected_exact_v<void, draw_function, widget_t>;

    template <typename widget_t>
    constexpr void detect_on_draw(widget_t* widget)
    {
        if constexpr (detail::has_draw_function<widget_t>) {
            detail::untyped_widget_data& _widget_data = detail::get_untyped_widget(*widget);
            _widget_data.drawer_command = draw_command();
            _widget_data.drawer = [widget](draw_command& command) {
                widget->draw(command);
            };
        }
    }
}

template <typename widget_t>
void on_draw(widget_t* widget, const std::function<void(draw_command&)>& draw_callback)
{
    if (!draw_callback)
        detail::throw_error<error_type::bad_usage>("resolve callback is nullptr");
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get(*widget);
    if (!_widget_data.drawer_command.has_value())
        _widget_data.drawer_command = draw_command();
    _widget_data.drawer = [widget](draw_command& command) {
        draw_callback(command);
    };
}

template <typename widget_t>
void must_draw(widget_t* widget)
{
    if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(widget)) == detail::widgets_context.accessors.end())
        detail::register_widget(&widget);
    detail::untyped_widget_data& _data = detail::get_untyped_widget(*widget);
    detail::widgets_context.drawables.emplace_back(_data);
}
}