#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that imposes different constraints on its child than it gets from its
    /// parent, possibly allowing the child to overflow the parent.
    struct OverflowBox {

        /// @brief How to align the child.
        OverflowBox& alignment(const Alignment value);

        /// @brief The widget below this widget in the tree.
        OverflowBox& child(const widget_id& value);

        /// @brief The maximum height constraint to give the child. Set this to std::nullopt (the
        /// default) to use the constraint from the parent instead.
        OverflowBox& maxHeight(const std::optional<float> value = std::nullopt);

        /// @brief The maximum width constraint to give the child. Set this to std::nullopt (the
        /// default) to use the constraint from the parent instead.
        OverflowBox& maxWidth(const std::optional<float> value = std::nullopt);

        /// @brief The minimum height constraint to give the child. Set this to std::nullopt (the
        /// default) to use the constraint from the parent instead.
        OverflowBox& minHeight(const std::optional<float> value = std::nullopt);

        /// @brief The minimum width constraint to give the child. Set this to std::nullopt (the
        /// default) to use the constraint from the parent instead.
        OverflowBox& minWidth(const std::optional<float> value = std::nullopt);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<widget_id> _childWidget = std::nullopt;
        Alignment _alignment = Alignment::center();
        std::optional<float> _maxHeight = std::nullopt;
        std::optional<float> _maxWidth = std::nullopt;
        std::optional<float> _minHeight = std::nullopt;
        std::optional<float> _minWidth = std::nullopt;
    };

}
}