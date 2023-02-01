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
#include <optional>
#include <typeindex>
#include <vector>

#include <staticgui/state/events_registry.hpp>

namespace staticgui {
namespace detail {

    struct animation_disabled {
    };

    template <typename value_t>
    struct animation_impl {
        event_impl<value_t> event;
    };

    struct animation_data {
        std::function<void()> tick = nullptr;
        std::unique_ptr<std::type_index> kind;
    };

    struct animations_registry {
        animations_registry();
        animations_registry(const animations_registry& other) = delete;
        animations_registry& operator=(const animations_registry& other) = delete;
        animations_registry(animations_registry&& other);
        animations_registry& operator=(animations_registry&& other);

        void tick_all();

        template <typename value_t>
        animation_impl<value_t>& make_animation_and_data();

        template <typename value_t>
        animation_data& get_data(animation_impl<value_t>& widget);

        template <typename value_t>
        animation_impl<value_t>& get_animation(animation_data& data);

        void iterate_datas(const std::function<void(animation_data&)>& iterate_callback);

        template <typename value_t>
        void iterate_animations(const std::function<void(animation_impl<value_t>&)>& iterate_callback);

    private:
        glue::registry _registry;
    };

}
}

#include <staticgui/state/animations_registry.inl>
