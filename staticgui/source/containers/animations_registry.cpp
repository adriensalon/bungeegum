//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/containers/animations_registry.hpp>

namespace staticgui {
namespace detail {

    animations_registry::animations_registry()
    {
    }

    animations_registry::animations_registry(animations_registry&& other)
    {
        *this = std::move(other);
    }

    animations_registry& animations_registry::operator=(animations_registry&& other)
    {
        _registry = std::move(other._registry);
        return *this;
    }

    void animations_registry::iterate_datas(const std::function<void(animation_data&)>& iterate_callback)
    {
        _registry.iterate_components<animation_data>([&](animation_data& _animation_data) {
            iterate_callback(_animation_data);
        });
    }
}
}