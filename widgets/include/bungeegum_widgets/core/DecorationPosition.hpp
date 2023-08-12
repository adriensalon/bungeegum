#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief Where to paint a box decoration.
    enum struct DecorationPosition {

        /// @brief Paint the box decoration behind the children.
        background,

        /// @brief Paint the box decoration in front of the children.
        foreground
    };

}
}