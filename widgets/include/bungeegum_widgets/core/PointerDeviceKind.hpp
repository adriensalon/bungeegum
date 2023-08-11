#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief The kind of pointer device.
    enum struct PointerDeviceKind {

        /// @brief A touch-based pointer device.
        /// @details The most common case is a touch screen. When the user is operating with a
        /// trackpad on iOS, clicking will also dispatch events with kind touch if
        /// UIApplicationSupportsIndirectInputEvents is not present in Info.plist or returns NO.
        touch,

        /// @brief A mouse-based pointer device.
        /// @details The most common case is a mouse on the desktop or Web. When the user is
        /// operating with a trackpad on iOS, moving the pointing cursor will also dispatch events
        ///  with kind mouse, and clicking will dispatch events with kind mouse if
        /// UIApplicationSupportsIndirectInputEvents is not present in Info.plist or returns NO.
        mouse
    };
}

}