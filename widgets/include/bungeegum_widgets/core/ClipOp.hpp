#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief Defines how a new clip region should be merged with the existing clip region.
    enum struct ClipOp {

        /// @brief Subtract the new region from the existing region.
        difference,

        /// @brief Intersect the new region from the existing region.
        intersect
    };

}
}