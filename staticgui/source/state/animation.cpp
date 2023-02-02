//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/animation.hpp>

namespace staticgui {
namespace detail {

    animation_registry::animation_registry()
    {
    }

    animation_registry::animation_registry(animation_registry&& other)
    {
        *this = std::move(other);
    }

    animation_registry& animation_registry::operator=(animation_registry&& other)
    {
        _registry = std::move(other._registry);
        return *this;
    }

    void animation_registry::tick(const float delta_milliseconds)
    {
        iterate_datas([&](animation_data& _animation_data) {
            _animation_data.tick(delta_milliseconds);
        });
    }

    void animation_registry::iterate_datas(const std::function<void(animation_data&)>& iterate_callback)
    {
        _registry.iterate_components<animation_data>([&](animation_data& _animation_data) {
            iterate_callback(_animation_data);
        });
    }
}
}