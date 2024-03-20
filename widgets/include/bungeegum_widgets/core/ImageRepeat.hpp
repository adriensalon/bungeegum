#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How to paint any portions of a box not covered by an image.
    enum struct ImageRepeat {

        /// @brief Repeat the image in both the x and y directions until the box is filled.
        repeat,

        /// @brief Repeat the image in the x direction until the box is filled horizontally.
        repeatX,

        /// @brief Repeat the image in the y direction until the box is filled vertically.
        repeatY,

        /// @brief Leave uncovered portions of the box transparent.
        noRepeat
    };

}
}