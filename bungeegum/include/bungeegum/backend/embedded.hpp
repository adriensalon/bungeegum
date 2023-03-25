#pragma once

#include <functional>

namespace bungeegum {

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
std::function<void()> embed(widget_t& widget);

}

#include <bungeegum/backend/embedded.inl>