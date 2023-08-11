#pragma once

#include <bungeegum_widgets/core/DismissDirection.hpp>
#include <bungeegum_widgets/core/DragStartBehavior.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief
    struct Dismissible {

        /// @brief
        using ConfirmDismissCallback = std::function<std::future<bool>(const DismissDirection)>;

        /// @brief
        Dismissible& background(const std::optional<runtime_widget>& value);

        /// @brief How this gesture detector should behave during hit testing when deciding how the
        /// hit test propagates to children and whether to consider targets behind this one.
        /// @details This defaults to HitTestBehavior.deferToChild if child is not null and
        /// HitTestBehavior.translucent if child is null.
        Dismissible& behavior(const HitTestBehavior value);

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        Dismissible& child(const runtime_widget& value);

        Dismissible& crossAxisEndOffset(const float1 value);

        Dismissible& direction(const DismissDirection value);

        Dismissible& dismissThresholds(const std::unordered_map<DismissDirection, float1>& value);

        Dismissible& dragStartBehavior(const DragStartBehavior& value);

        // ETC

    private:
        friend struct access;
        void interact(interact_command& command);
        void resolve(resolve_command& command);

        bool _absorb = false;
        std::optional<runtime_widget> _child = std::nullopt;
    };

}
}