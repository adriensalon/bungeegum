#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief
    struct PointerExitEvent {

        /// @brief Bit field using the *Button constants such as kPrimaryMouseButton,
        /// kSecondaryStylusButton, etc.
        const int buttons;
    };

}
}