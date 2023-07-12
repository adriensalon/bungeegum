#pragma once

#include <bungeegum/backend/standalone.fwd>

#if BUNGEEGUM_USE_STANDALONE

namespace bungeegum {

/// @brief
struct standalone_app {
    standalone_app() = delete;
    standalone_app(const standalone_app& other) = delete;
    standalone_app& operator=(const standalone_app& other) = delete;
    standalone_app(standalone_app&& other) = delete;
    standalone_app& operator=(standalone_app&& other) = delete;

    /// @brief
    static void color(const float4 rgba);

    /// @brief
    static void title(const std::string& description);

    /// @brief
    static float2 viewport();
};

/// @brief Starts an infinite loop
void launch(const runtime_widget& widget);
}

#endif