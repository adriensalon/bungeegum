//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <chrono>
#include <functional>
#include <optional>
#include <typeindex>
#include <vector>

#include <bungeegum/core/event.hpp>
#include <bungeegum/glue/curve.hpp>
#include <bungeegum/glue/lerp.hpp>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    struct typed_animation_data {
        std::unique_ptr<value_t> min_value = nullptr;
        std::unique_ptr<value_t> max_value = nullptr;
        typed_event_data<value_t> event;
        float playing_cursor_seconds = 0.f;
        float duration_seconds = 0.f;
    };

    struct untyped_animation_data {
        std::unique_ptr<std::type_index> kind;
        std::function<void(const std::chrono::milliseconds&)> tick = nullptr;
        bool is_playing = false;
        curve eval_curve { 0.f, 1.f, std::vector<float2> {}, false };
        float2 eval_point { 0.f, 0.f };
    };

    struct animations_registry {
        registry animations;

        void tick(const std::chrono::milliseconds& delta_time);
        void iterate(const std::function<void(untyped_animation_data&)>& iterate_callback);

        template <typename value_t>
        typed_animation_data<value_t>& create(events_registry& events);

        template <typename value_t>
        void destroy(const typed_animation_data<value_t>& animation);

        template <typename value_t>
        void start(typed_animation_data<value_t>& animation);

        template <typename value_t>
        void stop(typed_animation_data<value_t>& animation);

        template <typename value_t>
        void reset(typed_animation_data<value_t>& animation);

        template <typename value_t>
        void shape(typed_animation_data<value_t>& animation, const curve& animation_curve);

        template <typename value_t>
        void set_animation_min(typed_animation_data<value_t>& animation, value_t&& min_value);

        template <typename value_t>
        void set_animation_max(typed_animation_data<value_t>& animation, value_t&& max_value);

        template <typename value_t, typename duration_unit_t = std::chrono::seconds>
        void set_animation_duration(typed_animation_data<value_t>& animation, const unsigned int count);

        template <typename value_t>
        void attach_animation(typed_animation_data<value_t>& animation);

        template <typename value_t>
        void detach_animation(typed_animation_data<value_t>& animation);

        template <typename value_t>
        untyped_animation_data& get(typed_animation_data<value_t>& widget);

        template <typename value_t>
        typed_animation_data<value_t>& get(untyped_animation_data& data);

        template <typename value_t>
        void iterate(const std::function<void(typed_animation_data<value_t>&)>& iterate_callback);
    };

    inline animations_registry animations_context;

}

/// @brief
/// @details
enum struct animation_mode {
    forward,
    backward,
    repeat,
    loop,
    random
};

/// @brief
/// @tparam value_t
template <typename value_t>
struct animation {

    using lerpable_value = lerpable_t<value_t>;

    /// @brief
    using on_tick_callback = std::function<void(const value_t&)>;

    template <typename duration_unit_t = std::chrono::seconds>
    animation(
        const curve& curved_shape,
        lerpable_value&& min_value,
        lerpable_value&& max_value,
        const unsigned int duration_count = 1,
        const animation_mode mode = animation_mode::forward);
    animation(const animation& other);
    animation& operator=(const animation& other);
    animation(animation&& other);
    animation& operator=(animation&& other);
    ~animation();

    /// @brief
    /// @param value_changed_event
    animation& on_value_changed(const event<value_t>& value_changed_event);

    /// @brief
    /// @param value_changed_event
    animation& on_tick(const on_tick_callback& tick_callback);

    /// @brief
    /// @param value
    animation& on_tick(value_t& value, const bool must_draw = false, const bool must_resolve = false);

    /// @brief
    animation& start();

    /// @brief
    animation& stop();

    /// @brief
    animation& reset();

    /// @brief
    animation& shape(const curve& curved_shape);

    /// @brief
    animation& min(value_t&& min_value);

    /// @brief
    animation& max(value_t&& min_value);

    /// @brief
    template <typename duration_unit_t = std::chrono::seconds>
    animation& duration(const unsigned int count);

    /// @brief Transfers ownership of the underlying callback dispatcher back to this animation object
    /// @details This allows you to
    animation& attach();

    /// @brief Transfers ownership of the underlying callback dispatcher to the specified widget
    /// @details This allows you to let this animation object exit scope without destroying its
    /// callback dispatch subroutine. While an animation is detached to a widget both will be destroyed
    /// at the same time
    /// @tparam widget_t
    /// @param widget
    template <typename widget_t>
    animation& detach(widget_t& widget);

    /// @brief Transfers ownership of the underlying callback dispatcher to the internal registry
    /// @details This allows you to let this animation object exit scope without destroying its
    /// callback dispatch subroutine. While an animation is detached to the internal registry it will
    /// be destroyed when the application terminates
    animation& detach();

private:
    detail::typed_animation_data<value_t>& _data;
    // template <typename value_t>
    // friend struct animatable;
};

}

#include <bungeegum/core/animation.inl>
