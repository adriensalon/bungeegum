#pragma once

#include <functional>

namespace bungeegum {

/// @brief Starts an infinite loop
/// @tparam widget_t
/// @param widget
/// @param on_renderer_started
template <typename widget_t>
void launch(widget_t& widget, const std::function<void()>& on_renderer_started = nullptr);

}

#include <bungeegum/backend/standalone.inl>