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

#include <staticgui/glue/lerp.hpp>
#include <staticgui/state/curve.hpp>
#include <staticgui/state/event.hpp>


namespace staticgui {
namespace detail {

    template <typename value_t>
    struct animation_impl {
        std::unique_ptr<value_t> min_value = nullptr;
        std::unique_ptr<value_t> max_value = nullptr;
        event_impl<value_t> event;
        float playing_cursor_seconds = 0.f;
        float duration_seconds = 0.f;
    };

    struct animation_data {
        std::function<void(const float)> tick = nullptr;
        std::unique_ptr<std::type_index> kind;
        bool is_playing = false;
        curve_data curve;
        glue::simd_array<float, 2> t_curve = std::array<float, 2> { 0.f, 0.f };
    };

    struct animation_registry {
        animation_registry();
        animation_registry(const animation_registry& other) = delete;
        animation_registry& operator=(const animation_registry& other) = delete;
        animation_registry(animation_registry&& other);
        animation_registry& operator=(animation_registry&& other);

        void tick(const float delta_milliseconds);

        template <typename value_t>
        animation_impl<value_t>& make_animation_and_data(event_registry& events);

        template <typename value_t>
        void destroy_animation_and_data(const animation_impl<value_t>& animation);

        template <typename value_t>
        void start_animation(animation_impl<value_t>& animation);

        template <typename value_t>
        void stop_animation(animation_impl<value_t>& animation);

        template <typename value_t>
        void reset_animation(animation_impl<value_t>& animation);

        template <typename value_t>
        void shape_animation(animation_impl<value_t>& animation, const curve_data& curve);

        template <typename value_t>
        void set_animation_min(animation_impl<value_t>& animation, value_t&& min_value);

        template <typename value_t>
        void set_animation_max(animation_impl<value_t>& animation, value_t&& max_value);

        template <typename value_t, typename duration_unit_t = std::chrono::seconds>
        void set_animation_duration(animation_impl<value_t>& animation, const unsigned int count);

        template <typename value_t>
        void attach_animation(animation_impl<value_t>& animation);

        template <typename value_t>
        void detach_animation(animation_impl<value_t>& animation);

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

#include <staticgui/state/animation.inl>
