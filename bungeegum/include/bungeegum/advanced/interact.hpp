#pragma once

#include <bungeegum/advanced/interact.fwd>

namespace bungeegum {

/// @brief
/// @details
struct interact_command {

private:
    detail::interact_command_data _data;
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param interact_callback
template <typename widget_t>
void on_interact(widget_t* widget, const std::function<void(interact_command&)>& interact_callback);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void must_interact(widget_t* widget);

}

#include <bungeegum/advanced/interact.inl>
