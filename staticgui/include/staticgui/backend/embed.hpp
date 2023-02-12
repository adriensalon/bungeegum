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

namespace staticgui {

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
std::function<void()> embed(widget_t& widget);

}

#include <staticgui/backend/embed.inl>