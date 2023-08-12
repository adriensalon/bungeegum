#pragma once

#include <unordered_set>

#include <bungeegum_widgets/core/HitTestBehavior.hpp>
#include <bungeegum_widgets/core/PointerDeviceKind.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that detects gestures.
    /// @details Attempts to recognize gestures that correspond to its non-null callbacks. If this
    /// widget has a child, it defers to that child for its sizing behavior. If it does not have a
    /// child, it grows to fit the parent instead.
    struct GestureDetector {

        /// @brief How this gesture detector should behave during hit testing when deciding how the
        /// hit test propagates to children and whether to consider targets behind this one.
        /// @details This defaults to HitTestBehavior.deferToChild if child is not null and
        /// HitTestBehavior.translucent if child is null.
        GestureDetector& behavior(const HitTestBehavior value);

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        GestureDetector& child(const runtime_widget& value);

    private:
        friend struct access;
        void interact(interact_command& command);
        void resolve(resolve_command& command);

        HitTestBehavior _behavior = HitTestBehavior::deferToChild;
    };

}
}