#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief
    struct PointerEnterEvent {

        /// @brief Bit field using the *Button constants such as kPrimaryMouseButton,
        /// kSecondaryStylusButton, etc.
        const int buttons;
    };

}
}