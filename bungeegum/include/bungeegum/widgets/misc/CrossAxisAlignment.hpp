#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How the children should be placed along the cross axis in a flex layout.
    enum struct CrossAxisAlignment {

        /// @brief Place the children with their start edge aligned with the start side of the
        /// cross axis.
        /// @details For example, in a column (a flex with a vertical axis) whose TextDirection is
        /// TextDirection.ltr, this aligns the left edge of the children along the left edge of the
        /// column.
        /// If this value is used in a horizontal direction, a TextDirection must be available to
        /// determine if the start is the left or the right.
        /// If this value is used in a vertical direction, a VerticalDirection must be available to
        /// determine if the start is the top or the bottom.
        start,

        /// @brief Place the children as close to the end of the cross axis as possible.
        /// @details For example, in a column (a flex with a vertical axis) whose TextDirection is
        /// TextDirection.ltr, this aligns the right edge of the children along the right edge of the
        /// column.
        /// If this value is used in a horizontal direction, a TextDirection must be available to
        /// determine if the end is the left or the right.
        /// If this value is used in a vertical direction, a VerticalDirection must be available to
        /// determine if the end is the top or the bottom.
        end,

        /// @brief Place the children so that their centers align with the middle of the cross axis.
        /// @details This is the default cross-axis alignment.
        center,

        /// @brief Require the children to fill the cross axis.
        /// @details This causes the constraints passed to the children to be tight in the cross
        /// axis.
        stretch,

        // baseline,
    };

}
}