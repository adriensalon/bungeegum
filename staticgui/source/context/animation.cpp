//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <iostream>

#include <staticgui/context/animation.hpp>

namespace staticgui {
namespace detail {

    void animations_registry::tick(const std::chrono::milliseconds& delta_time)
    {
        iterate([&](untyped_animation_data& _data) {
            _data.tick(delta_time);
        });
    }

    void animations_registry::iterate(const std::function<void(untyped_animation_data&)>& iterate_callback)
    {
        animations.iterate<untyped_animation_data>(iterate_callback);
    }
}

}