#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that rotates its child by a integral number of quarter turns.
    /// @details Unlike Transform, which applies a transform just prior to painting, this object 
    /// applies its rotation prior to layout, which means the entire rotated box consumes only as 
    /// much space as required by the rotated child.
    struct RotatedBox {

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        RotatedBox& child(const widget_id& value);

        /// @brief The number of clockwise quarter turns the child should be rotated.
        RotatedBox& quarterTurns(const std::size_t value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        std::optional<widget_id> _childWidget = std::nullopt;
        std::size_t _quarterTurns = 0u;
        SERIALIZE_FIELDS(_quarterTurns)
    };

}
}