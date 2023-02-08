//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <variant>

#include <staticgui/state/animation.hpp>

namespace staticgui {
namespace detail {

    template <typename value_t>
    struct animatable_data {
        std::variant<value_t, animation_impl<value_t>> value;
        bool is_animation;
    };
}
}
