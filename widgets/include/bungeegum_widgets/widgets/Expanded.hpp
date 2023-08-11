#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/FlexFit.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that expands a child of a Row, Column, or Flex so that the child fills the
    /// available space.
    /// @details Using an Expanded widget makes a child of a Row, Column, or Flex expand to fill
    /// the available space along the main axis (e.g., horizontally for a Row or vertically for a
    /// Column). If multiple children are expanded, the available space is divided among them
    /// according to the flex factor. An Expanded widget must be a descendant of a Row, Column, or
    /// Flex.
    struct Expanded {

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        FittedBox& child(const runtime_widget& value);

        /// @brief How a flexible child is inscribed into the available space.
        /// @details If flex is non-zero, the fit determines whether the child fills the space the
        /// parent makes available during layout. If the fit is FlexFit.tight, the child is
        /// required to fill the available space. If the fit is FlexFit.loose, the child can be at
        /// most as large as the available space (but is allowed to be smaller).
        FittedBox& fit(const FlexFit value);

        /// @brief The flex factor to use for this child.
        /// @details If zero, the child is inflexible and determines its own size. If non-zero, the
        /// amount of space the child's can occupy in the main axis is determined by dividing the
        /// free space (after placing the inflexible children) according to the flex factors of the
        /// flexible children.
        FittedBox& flex(const uint1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        // std::optional<runtime_widget> _childWidget = std::nullopt;
        uint1 _flex = 1;
        FlexFit _fit = FlexFit::tight;
    };

}
}