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

    using lerpable_value = detail::lerpable_t<value_t>;

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
    /// @param tick_callback
    /// @return
    animation& on_tick(const on_tick_callback& tick_callback);

    /// @brief
    /// @param value
    /// @param must_draw
    /// @param must_resolve
    /// @return
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
