#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/HitTestBehavior.hpp>
#include <bungeegum_widgets/core/PointerEnterEvent.hpp>
#include <bungeegum_widgets/core/PointerExitEvent.hpp>
#include <bungeegum_widgets/core/PointerHoverEvent.hpp>
#include <bungeegum_widgets/core/SystemMouseCursor.hpp>

namespace bungeegum {
namespace widgets {

    struct MouseRegion {

        /// @brief Signature for listening to PointerEnterEvent events.
        using PointerEnterEventListener = std::function<void(const PointerEnterEvent&)>;

        /// @brief Signature for listening to PointerExitEvent events.
        using PointerExitEventListener = std::function<void(const PointerExitEvent&)>;

        /// @brief Signature for listening to PointerHoverEvent events.
        using PointerHoverEventListener = std::function<void(const PointerHoverEvent&)>;

        /// @brief How to behave during hit testing.
        /// @details This defaults to HitTestBehavior.deferToChild if child is not null and
        /// HitTestBehavior.translucent if child is null.
        MouseRegion& hitTestBehavior(const HitTestBehavior value);

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        MouseRegion& child(const std::optional<runtime_widget>& value);

        /// @brief
        MouseRegion& cursor(const SystemMouseCursor value);

        /// @brief
        MouseRegion& onPointerEnter(const PointerEnterEventListener& value);

        /// @brief
        MouseRegion& onPointerExit(const PointerExitEventListener& value);

        /// @brief Triggered when a pointer moves into a position within this widget without
        /// buttons pressed.
        /// @details Usually this is only fired for pointers which report their location when not
        /// down (e.g. mouse pointers). Certain devices also fire this event on single taps in
        /// accessibility mode. This callback is not triggered by the movement of the widget. The
        /// time that this callback is triggered is during the callback of a pointer event, which
        /// is always between frames.
        MouseRegion& onPointerHover(const PointerHoverEventListener& value);

        /// @brief Whether this widget should prevent other MouseRegions visually behind it from
        /// detecting the pointer.
        MouseRegion& opaque(const bool value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
    };

}
}