#pragma once

#include <bungeegum_widgets/core/BlendMode.hpp>
#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    struct Paint {

        Paint() = default;
        Paint(const Paint& other) = default;
        Paint& operator=(const Paint& other) = default;
        Paint(Paint&& other) = default;
        Paint& operator=(Paint&& other) = default;

        /// @brief A blend mode to apply when a shape is drawn or a layer is composited.
        BlendMode blendMode = BlendMode::srcOver;

        /// @brief
        Color color = {};

        // private:
    };
}

// largest ?
}