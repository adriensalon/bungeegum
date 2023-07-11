#pragma once

#include <bungeegum/backend/standalone.fwd>

namespace bungeegum {

/// @brief
struct bungeegum_app {
    bungeegum_app() = delete;
    bungeegum_app(const bungeegum_app& other) = delete;
    bungeegum_app& operator=(const bungeegum_app& other) = delete;
    bungeegum_app(bungeegum_app&& other) = delete;
    bungeegum_app& operator=(bungeegum_app&& other) = delete;

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