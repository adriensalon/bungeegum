//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>

namespace bungeegum {

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void launch(widget_t& widget, const std::function<void()>& on_renderer_started = nullptr);

}

#include <bungeegum/backend/launch.inl>