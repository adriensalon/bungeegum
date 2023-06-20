#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief The two cardinal directions in two dimensions.
    /// @details The axis is always relative to the current coordinate space. This means, for
    /// example, that a horizontal axis might actually be diagonally from top right to bottom left,
    /// due to some local Transform applied to the scene.
    enum struct Axis {

        /// @brief Left and right.
        horizontal,

        /// @brief Up and down.
        vertical
    };

}
}