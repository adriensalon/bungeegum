#pragma once

#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    template <typename widget_t>
    using draw_function = decltype(std::declval<widget_t>().draw(std::declval<draw_command&>()));

    template <typename widget_t>
    constexpr bool has_draw_function_v = is_detected_exact_v<void, draw_function, widget_t>;

    template <typename widget_t>
    constexpr void detect_on_draw(widget_t* widget)
    {
        if constexpr (detail::has_draw_function_v<widget_t>) {
            detail::untyped_widget_data& _widget_data = detail::widgets_context.get(*widget);
            _widget_data.widget_drawer_command = draw_command();
            _widget_data.widget_drawer = [widget](draw_command& command) { // [=widget] otherwise we pass a reference to ptr
                widget->draw(command);
            };
            std::cout << "YES \n";
        }
    }
}

/// @brief
/// @tparam widget_t
/// @param widget
/// @param draw_callback
template <typename widget_t>
void on_draw(widget_t* widget, const std::function<void(draw_command&)>& draw_callback)
{
}

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void must_draw(widget_t* widget)
{
    // detail::widgets_context.must_draw(*widget);
    if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(&widget)) == detail::widgets_context.accessors.end())
        detail::register_widget(&widget);
    detail::untyped_widget_data& _data = detail::widgets_context.get(widget);
    detail::widgets_context.must_draw_heads.emplace_back(std::pair<std::reference_wrapper<detail::untyped_widget_data>, bool> { _data, true });
}

}