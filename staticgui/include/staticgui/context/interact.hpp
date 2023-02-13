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
#include <vector>

namespace staticgui {

/// @brief
/// @details
struct interact_command {
    interact_command();
    interact_command(const interact_command& other);
    interact_command& operator=(const interact_command& other);
    interact_command(interact_command&& other);
    interact_command& operator=(interact_command&& other);
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param draw_callback
template <typename widget_t>
void on_interact(widget_t* widget, const std::function<void(interact_command&)>& interact_callback);

}

// #include <staticgui/context/interact.inl>
