#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/misc/EdgeInsets.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that insets its child by the given padding.
    /// @details When passing layout constraints to its child, padding shrinks the constraints by
    /// the given padding, causing the child to layout at a smaller size. Padding then sizes
    /// itself to its child's size, inflated by the padding, effectively creating empty space
    /// around the child.
    struct Padding {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        Padding& child(child_widget_t& value)
        {
            _child = adopt(this, value);
            return *this;
        }

        /// @brief The amount of space by which to inset the child.
        Padding& padding(const EdgeInsets value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<adopted_widget> _child = std::nullopt;
        EdgeInsets _edgeInsets = EdgeInsets::all(0.f);
    };

}
}