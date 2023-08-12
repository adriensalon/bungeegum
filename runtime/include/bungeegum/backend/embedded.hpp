#pragma once

#include <bungeegum/backend/embedded.fwd>

#if BUNGEEGUM_USE_EMBEDDED

namespace bungeegum {

/// @brief
struct bungeegum_functor {

    /// @brief
    inline void operator()();

    /// @brief
    /// @param size
    /// @param fullscreen
    inline void notify_window_resized(const float2 size, const bool fullscreen);

    /// @brief
    /// @param absolute_position
    /// @param relative_position
    inline void notify_mouse_moved(const float2 absolute_position, const float2 relative_position);

private:
    detail::bungeegum_functor_data _data;
    template <typename widget_t>
    friend bungeegum_functor embed(widget_t& widget);
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
bungeegum_functor embed(widget_t& widget);
}

#include <bungeegum/backend/embedded.inl>

#endif