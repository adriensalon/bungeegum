#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/BoxDecoration.hpp>
#include <bungeegum_widgets/core/DecorationPosition.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that paints a Decoration either before or after its child paints.
    /// @details Container insets its child by the widths of the borders; this widget does not.
    /// Commonly used with BoxDecoration. The child is not clipped. To clip a child to the shape
    /// of a particular ShapeDecoration, consider using a ClipPath widget.
    struct DecoratedBox {

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        DecoratedBox& child(const runtime_widget& value);

        /// @brief
        DecoratedBox& decoration(const BoxDecoration& value);

        /// @brief
        DecoratedBox& position(const DecorationPosition value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        // std::optional<runtime_widget> _childWidget = std::nullopt;
        DecorationPosition _position = DecorationPosition::background;
    };

}
}