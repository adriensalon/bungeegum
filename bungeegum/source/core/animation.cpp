#pragma once

#include <iostream>

#include <bungeegum/core/animation.hpp>

namespace bungeegum {
namespace detail {

    // void animations_registry::tick(const std::chrono::milliseconds& delta_time)
    // {
    //     iterate([&](untyped_animation_data& _data) {
    //         _data.tick(delta_time);
    //     });
    // }

    // void animations_registry::iterate(const std::function<void(untyped_animation_data&)>& iterate_callback)
    // {
    //     animations.iterate<untyped_animation_data>(iterate_callback);
    // }
}

}