#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief Creates a new widget managed by bungeegum
/// @tparam widget_t Custom type for the widget. It must be at least copy or move constructible.
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return Returns a reference to the new widget
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args);

/// @brief
/// @tparam widget_t
/// @tparam ...children_widgets_t
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void adopt(widget_t* widget, children_widgets_t&... children_widgets);

/// @brief
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void abandon(widget_t* widget, children_widgets_t&... children_widgets);

/// @brief
/// @tparam widget_t
/// @param iterate_function
template <typename widget_t>
void iterate(const std::function<void(widget_t&)>& iterate_function);
}

#include <bungeegum/core/widget.inl>
