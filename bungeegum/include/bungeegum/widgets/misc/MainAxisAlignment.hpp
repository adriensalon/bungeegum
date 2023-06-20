#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How the children should be placed along the main axis in a flex layout.
    enum struct MainAxisAlignment {

        /// @brief Place the children as close to the start of the main axis as possible.
        /// @details If this value is used in a horizontal direction, a TextDirection must be
        /// available to determine if the start is the left or the right.
        /// If this value is used in a vertical direction, a VerticalDirection must be available
        /// to determine if the start is the top or the bottom.
        start,

        /// @brief Place the children as close to the end of the main axis as possible.
        /// @details If this value is used in a horizontal direction, a TextDirection must be
        /// available to determine if the end is the left or the right.
        /// If this value is used in a vertical direction, a VerticalDirection must be available
        /// to determine if the end is the top or the bottom.
        end,

        /// @brief Place the children as close to the start of the main axis as possible.
        center,

        /// @brief Place the free space evenly between the children.
        spaceBetween,

        /// @brief Place the free space evenly between the children as well as half of that space
        /// before and after the first and last child.
        spaceAround,

        /// @brief Place the free space evenly between the children as well as before and after
        /// the first and last child.
        spaceEvenly
    };

}
}