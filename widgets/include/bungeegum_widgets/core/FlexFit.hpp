#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How the child is inscribed into the available space.
    enum struct FlexFit {

        /// @brief The child is forced to fill the available space.
        /// @details The Expanded widget assigns this kind of FlexFit to its child.
        tight,

        /// @brief The child can be at most as large as the available space (but is allowed to be
        /// smaller).
        /// @details The Flexible widget assigns this kind of FlexFit to its child.
        loose
    };
}
}