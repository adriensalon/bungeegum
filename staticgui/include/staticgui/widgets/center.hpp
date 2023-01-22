//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/staticgui.hpp>

namespace staticgui {

/// @brief
/// @tparam child_widget_t
template <typename child_widget_t>
struct center {
    center(const child_widget_t& child)
    {
        register_child<center, child_widget_t>(this, child);
    }
    void draw(const context& context)
    {
    }
};
}