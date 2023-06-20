#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How much space should be occupied in the main axis.
    /// @details During a flex layout, available space along the main axis is allocated to
    /// children. After allocating space, there might be some remaining free space. This value
    /// controls whether to maximize or minimize the amount of free space, subject to the incoming
    /// layout constraints.
    enum struct MainAxisSize {

        min,

        max
    };

}
}