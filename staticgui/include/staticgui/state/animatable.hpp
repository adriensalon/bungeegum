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
        std::optional<std::reference_wrapper<animation_impl<value_t>>> animation;
        std::optional<value_t> value;
        bool is_animated;
    };

    template <typename value_t>
    void assign_animatable(const animatable_data<value_t>& animatable, value_t& target_value)
    {
        // if (animatable.is_animated) {
        // 	animation_impl<value_t>& _animation = animatable.animation.value().get();

        //     start().detach().on_tick(([&](const value_t& _value_changed) {
        //         target_value = _value_changed;
        //     }));
        // } else
        //     target_value = animatable.value.value();
    }
}
}
