#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief
/// @tparam widget_t
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return
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
}

#include <bungeegum/core/widget.inl>
