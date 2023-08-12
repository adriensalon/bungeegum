#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief The direction in which a Dismissible can be dismissed.
    enum struct DismissDirection {

        /// @brief The Dismissible can be dismissed by dragging either up or down.
        vertical,

        /// @brief The Dismissible can be dismissed by dragging either left or right.
        horizontal,

        /// @brief The Dismissible can be dismissed by dragging in the reverse of the reading
        /// direction (e.g., from right to left in left-to-right languages).
        endToStart,

        /// @brief The Dismissible can be dismissed by dragging in the reading direction (e.g.,
        /// from left to right in left-to-right languages).
        startToEnd,

        /// @brief The Dismissible can be dismissed by dragging up only.
        up,

        /// @brief The Dismissible can be dismissed by dragging down only.
        down,

        /// @brief The Dismissible cannot be dismissed by dragging.
        none
    };

}
}