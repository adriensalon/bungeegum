//   _
//  | |__   _   _  _ __    __ _   ___   ___   __ _  _   _  _ __ ___
//  | '_ \ | | | || '_ \  / _` | / _ \ / _ \ / _` || | | || '_ ` _ \ 
//  | |_) || |_| || | | || (_| ||  __/|  __/| (_| || |_| || | | | | |
//  |_.__/  \__,_||_| |_| \__, | \___| \___| \__, | \__,_||_| |_| |_|
//                        |___/              |___/

#pragma once

#include <functional>
#include <vector>

namespace bungeegum {

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
/// @param interact_callback
template <typename widget_t>
void on_interact(widget_t* widget, const std::function<void(interact_command&)>& interact_callback);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void must_interact(widget_t* widget);

}

// #include <bungeegum/context/interact.inl>
