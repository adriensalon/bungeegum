#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return Returns a reference to the new widget
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args);

/// @brief Destroys a widget created with the make function. References to it will no longer
/// be valid.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @param widget
template <typename widget_t>
void unmake(widget_t& widget);

/// @brief Registers widgets as children of another widget.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @tparam ...children_widgets_t Custom types for the children widgets. Must be at least copy or
/// move constructible.
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void adopt(widget_t* widget, children_widgets_t&... children_widgets);

/// @brief Unregisters a parenting relationship
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void abandon(widget_t* widget, children_widgets_t&... children_widgets);
}

#include <bungeegum/core/widget.inl>
