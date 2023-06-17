#pragma once

#include <functional>

#include <bungeegum/glue/simd.hpp>

namespace bungeegum {

/// @brief
struct bungeegum_app {

    /// @brief
    static void color(const float4 rgba);

    /// @brief
    static void title(const std::string& description);
};

/// @brief Starts an infinite loop
/// @tparam widget_t
/// @param widget
/// @param on_renderer_started
template <typename widget_t>
void launch(widget_t& widget, const std::function<void()>& on_renderer_started = nullptr);

}

#include <bungeegum/backend/standalone.inl>