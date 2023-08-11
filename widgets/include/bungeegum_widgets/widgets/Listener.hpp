#pragma once

#include <bungeegum_widgets/core/HitTestBehavior.hpp>
#include <bungeegum_widgets/core/PointerCancelEvent.hpp>
#include <bungeegum_widgets/core/PointerDeviceKind.hpp>
#include <bungeegum_widgets/core/PointerDownEvent.hpp>
#include <bungeegum_widgets/core/PointerHoverEvent.hpp>
#include <bungeegum_widgets/core/PointerMoveEvent.hpp>
#include <bungeegum_widgets/core/PointerPanZoomEndEvent.hpp>
#include <bungeegum_widgets/core/PointerPanZoomStartEvent.hpp>
#include <bungeegum_widgets/core/PointerPanZoomUpdateEvent.hpp>
#include <bungeegum_widgets/core/PointerSignalEvent.hpp>
#include <bungeegum_widgets/core/PointerUpEvent.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that calls callbacks in response to common pointer events.
    /// @details It listens to events that can construct gestures, such as when the pointer is
    /// pressed, moved, then released or canceled. It does not listen to events that are exclusive
    /// to mouse, such as when the mouse enters, exits or hovers a region without pressing any
    /// buttons. For these events, use MouseRegion. Rather than listening for raw pointer events,
    /// consider listening for higher-level gestures using GestureDetector.
    struct Listener {

        /// @brief Signature for listening to PointerCancelEvent events.
        using PointerCancelEventListener = std::function<void(const PointerCancelEvent&)>;

        /// @brief Signature for listening to PointerDownEvent events.
        using PointerDownEventListener = std::function<void(const PointerDownEvent&)>;

        /// @brief Signature for listening to PointerMoveEvent events.
        using PointerMoveEventListener = std::function<void(const PointerMoveEvent&)>;

        /// @brief Signature for listening to PointerHoverEvent events.
        using PointerHoverEventListener = std::function<void(const PointerHoverEvent&)>;

        /// @brief Signature for listening to PointerPanZoomEndEvent events.
        using PointerPanZoomEndEventListener = std::function<void(const PointerPanZoomEndEvent&)>;

        /// @brief Signature for listening to PointerPanZoomStartEvent events.
        using PointerPanZoomStartEventListener = std::function<void(const PointerPanZoomStartEvent&)>;

        /// @brief Signature for listening to PointerPanZoomUpdateEvent events.
        using PointerPanZoomUpdateEventListener = std::function<void(const PointerPanZoomUpdateEvent&)>;

        /// @brief Signature for listening to PointerSignalEvent events.
        using PointerSignalEventListener = std::function<void(const PointerSignalEvent&)>;

        /// @brief Signature for listening to PointerUpEvent events.
        using PointerUpEventListener = std::function<void(const PointerUpEvent&)>;

        /// @brief How to behave during hit testing.
        /// @details This defaults to HitTestBehavior.deferToChild if child is not null and
        /// HitTestBehavior.translucent if child is null.
        Listener& behavior(const HitTestBehavior value);

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        Listener& child(const std::optional<runtime_widget>& value);

        /// @brief Called when the input from a pointer that triggered an onPointerDown is no
        /// longer directed towards this receiver.
        Listener& onPointerCancel(const PointerCancelEventListener& value);

        /// @brief Signature for listening to PointerDownEvent events.
        /// @details Used by Listener and RenderPointerListener.
        Listener& onPointerDown(const PointerDownEventListener& value);

        /// @brief Called when a pointer that has not triggered an onPointerDown changes position.
        /// @details This is only fired for pointers which report their location when not down
        /// (e.g. mouse pointers, but not most touch pointers).
        Listener& onPointerHover(const PointerHoverEventListener& value);

        /// @brief Called when a pointer that triggered an onPointerDown changes position.
        Listener& onPointerMove(const PointerMoveEventListener& value);

        /// @brief Called when a pan/zoom finishes.
        Listener& onPointerPanZoomEnd(const PointerPanZoomEndEventListener& value);

        /// @brief Called when a pan/zoom begins such as from a trackpad gesture.
        Listener& onPointerPanZoomStart(const PointerPanZoomStartEventListener& value);

        /// @brief Called when a pan/zoom is updated.
        Listener& onPointerPanZoomUpdate(const PointerPanZoomUpdateEventListener& value);

        /// @brief Called when a pointer signal occurs over this object.
        Listener& onPointerSignal(const PointerSignalEventListener& value);

        /// @brief Called when a pointer that triggered an onPointerDown is no longer in contact
        /// with the screen.
        Listener& onPointerUp(const PointerUpEventListener& value);

    private:
        friend struct access;
        void interact(interact_command& command);
        void resolve(resolve_command& command);

        HitTestBehavior _behavior = HitTestBehavior::deferToChild;
        std::optional<runtime_widget> _child = std::nullopt;
        PointerCancelEventListener _onPointerCancel = nullptr;
        PointerDownEventListener _onPointerDown = nullptr;
        PointerHoverEventListener _onPointerHover = nullptr;
        PointerMoveEventListener _onPointerMove = nullptr;
        PointerPanZoomEndEventListener _onPointerPanZoomEnd = nullptr;
        PointerPanZoomStartEventListener _onPointerPanZoomStart = nullptr;
        PointerPanZoomUpdateEventListener _onPointerPanZoomUpdate = nullptr;
        PointerSignalEventListener _onPointerSignal = nullptr;
        PointerUpEventListener _onPointerUp = nullptr;
    };

}
}