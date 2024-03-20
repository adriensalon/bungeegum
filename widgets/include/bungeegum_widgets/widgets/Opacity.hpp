#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that makes its child partially transparent.
    /// @details With Flutter, this class paints its child into an intermediate buffer and then 
    /// blends the child back into the scene partially transparent. For values of opacity other
    /// than 0.0 and 1.0, it is relatively expensive. With this implementation all draw calls
    /// will multiply colors alpha components by an opacity factor. (must compare cpu vs gpu)
    struct Opacity {

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        Opacity& child(const widget_id& value);

        /// @brief The fraction to scale the child's alpha value.
        /// @details An opacity of one is fully opaque. An opacity of zero is fully transparent 
        /// (i.e., invisible).
        Opacity& opacity(const float value);

    private:
        friend struct access;
        void draw(draw_command& command);
        std::optional<widget_id> _childWidget = std::nullopt;
        float _opacity = 1.f;
        SERIALIZE_FIELDS(_opacity)
    };

}
}