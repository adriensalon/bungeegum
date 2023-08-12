#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief Configuration of offset passed to DragStartDetails.
    enum struct DragStartBehavior {

        /// @brief Set the initial offset at the position where the first down event was detected.
        down,

        /// @brief Set the initial position at the position where this gesture recognizer won the
        /// arena.
        start
    };
}

}