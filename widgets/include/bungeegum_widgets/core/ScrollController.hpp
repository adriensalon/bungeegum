#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct Offset;

    struct Size;

    /// @brief An immutable, 2D, axis-aligned, floating-point rectangle whose coordinates
    /// are relative to a given origin.
    /// @details Imitates https://api.flutter.dev/flutter/dart-ui/Rect-class.html
    struct ScrollController {

        // using ScrollControllerCallback = std::function<void(const ScrollPosition&)>

        /// @brief A Rect can be created from an Offset and a Size.
        // ScrollController(const float& initialScrollOffset = 0.f, const bool keepScrollOffset = true, );

    };
}

}