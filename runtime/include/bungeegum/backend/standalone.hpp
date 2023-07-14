#pragma once

#include <bungeegum/backend/standalone.fwd>

namespace bungeegum {

/// @brief Static methods of this struct allow the user to modify and get information about the
/// context created by the launch() function.
/// @details No instance of this struct can be created, copied or moved. Static methods of this
/// struct fallback to no-op if BUNGEEGUM_ENABLE_STANDALONE is not defined to 1.
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

#if BUNGEEGUM_USE_STANDALONE

/// @brief Starts an infinite loop
void launch(const runtime_widget& widget);

#endif
}
