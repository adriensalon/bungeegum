#pragma once

#include <bungeegum/backend/standalone.fwd>

namespace bungeegum {

/// @brief
struct bungeegum_app {

    /// @brief
    static void color(const float4 rgba);

    /// @brief
    static void title(const std::string& description);

    /// @brief
    static float2 viewport();
};

/// @brief Starts an infinite loop
void launch(const runtime_widget& widget, const std::function<void()>& on_renderer_started = nullptr);
}

#include <bungeegum/backend/standalone.inl>