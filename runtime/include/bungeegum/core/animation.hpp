#pragma once

#include <bungeegum/core/animation.fwd>
#include <bungeegum/core/event.hpp>

namespace bungeegum {

/// @brief TODO GO SEE FLUTTER START / STOP / RESET BIZARRE PEUT ETRE
/// @details
enum struct animation_mode {
    forward,
    backward,
    repeat,
    loop,
    random
};

/// @brief Animation objects use curves to update variables and can be passed around the user
/// code with events or typed callbacks. Animations will be fired each frame when started.
/// @details Animation object instances can be copied (deep copy but the new animation is
/// stopped) and moved.
/// @tparam value_t is the lerpable type to use as argument for callbacks after interpolation.
template <typename value_t>
struct animation {

    /// @brief Instances of this type are the callback objects.
    using on_value_changed_callback = std::function<void(const value_t&)>;

    /// @brief Sets the maximum value corresponding to y = 1.f on the curve.
    animation& max(value_t&& min_value);

    /// @brief Sets the minimum value corresponding to y = 0.f on the curve.
    animation& min(value_t&& min_value);

    /// @brief Emplaces new callbacks from an event to be fired when the animation is playing and
    /// the value has changed.
    animation& on_value_changed(const event<value_t>& value_changed_event);

    /// @brief Emplaces a new callback to be fired when the animation is playing and the value has
    /// changed.
    animation& on_value_changed(const on_value_changed_callback& tick_callback);

    /// @brief Resets the animation.
    animation& reset();

    /// @brief Sets the animation shape from a curve.
    animation& shape(const curve& curved_shape);

    /// @brief Starts the animation.
    animation& start();

    /// @brief Stops the animation.
    animation& stop();

    /// @brief Sets the duration.
    /// @tparam duration_unit_t is the type from std::chrono to use as unit.
    template <typename duration_unit_t = std::chrono::seconds>
    animation& duration(const unsigned int count);

private:
    detail::animation_data<value_t> _data;
};
}

#include <bungeegum/core/animation.inl>
