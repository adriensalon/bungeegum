#pragma once

#include <bungeegum/core/animation.fwd>

namespace bungeegum {

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

    /// @brief
    using lerpable_value = detail::lerpable_t<value_t>;

    /// @brief
    using on_tick_callback = std::function<void(const value_t&)>;

    /// @brief
    /// @tparam duration_unit_t
    /// @param curved_shape
    /// @param min_value
    /// @param max_value
    /// @param duration_count
    /// @param mode
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
    /// @param tick_callback
    animation& on_tick(const on_tick_callback& tick_callback);

    /// @brief
    /// @param value
    /// @param must_draw
    /// @param must_resolve
    animation& on_tick(value_t& value, const bool must_draw = false, const bool must_resolve = false);

    /// @brief
    animation& start();

    /// @brief
    animation& stop();

    /// @brief
    animation& reset();

    /// @brief
    /// @param curved_shape
    animation& shape(const curve& curved_shape);

    /// @brief
    /// @param min_value
    animation& min(value_t&& min_value);

    /// @brief
    /// @param min_value
    animation& max(value_t&& min_value);

    /// @brief
    /// @tparam duration_unit_t
    /// @param count
    template <typename duration_unit_t = std::chrono::seconds>
    animation& duration(const unsigned int count);

private:
    detail::typed_animation_data<value_t>& _data; // GO VALUE
};

// make animation
}

#include <bungeegum/core/animation.inl>
