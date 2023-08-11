#pragma once

#include <bungeegum/core/runtime.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief
    struct AbsorbPointer {

        /// @brief Whether this widget absorbs pointers during hit testing.
        AbsorbPointer& absorb(const bool value);

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        AbsorbPointer& child(const std::optional<runtime_widget>& value);

    private:
        friend struct access;
        void interact(interact_command& command);
        void resolve(resolve_command& command);

        bool _absorb = false;
        std::optional<runtime_widget> _child = std::nullopt;
    };

}
}