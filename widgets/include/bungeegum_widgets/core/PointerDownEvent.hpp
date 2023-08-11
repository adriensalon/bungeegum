#pragma once

#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/PointerDeviceKind.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief The pointer has made contact with the device.
    struct PointerDownEvent {

        /// @brief Bit field using the *Button constants such as kPrimaryMouseButton,
        /// kSecondaryStylusButton, etc.
        const int buttons;

        /// @brief Distance in logical pixels that the pointer moved since the last
        /// PointerMoveEvent or PointerHoverEvent.
        /// @details This value is always 0.0 for down, up, and cancel events.
        const Offset delta;

        /// @brief Unique identifier for the pointing device, reused across interactions.
        const int device;

        /// @brief The distance of the detected object from the input surface.
        const float1 distance;

        /// @brief The maximum value that distance can return for this pointer.
        const float1 distanceMax;

        /// @brief The maximum value that distance can return for this pointer.
        const float1 distanceMin;

        /// @brief Set if the pointer is currently down.
        const bool1 down;

        /// @brief The kind of input device for which the event was generated.
        const PointerDeviceKind kind;

        /// @brief The delta transformed into the event receiver's local coordinate system
        /// according to transform.
        const Offset localDelta;

        /// @brief Set if an application from a different security domain is in any way obscuring
        /// this application's window.
        // const bool obscured;

        /// @brief The orientation angle of the detected object, in radians.
        const float1 orientation;
    };

}
}