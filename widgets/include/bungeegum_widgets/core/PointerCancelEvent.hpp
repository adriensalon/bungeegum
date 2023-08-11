#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief
    struct PointerCancelEvent {

        /// @brief Bit field using the *Button constants such as kPrimaryMouseButton,
        /// kSecondaryStylusButton, etc.
        const int buttons;
    };

}
}
}
}