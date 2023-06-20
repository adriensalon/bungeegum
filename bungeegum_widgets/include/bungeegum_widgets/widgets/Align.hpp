#pragma once

// https://api.flutter.dev/flutter/widgets/Align-class.html

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/misc/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that aligns its child within itself and optionally sizes itself based on
    /// the child's size. For example, to align a box at the bottom right, you would pass this box
    /// a tight constraint that is bigger than the child's natural size, with an alignment of
    /// Alignment::bottomRight()
    /// @details This widget will be as big as possible if its dimensions are constrained and
    /// widthFactor and heightFactor are null. If a dimension is unconstrained and the
    /// corresponding size factor is null then the widget will match its child's size in that
    /// dimension. If a size factor is non-null then the corresponding dimension of this widget will
    /// be the product of the child's dimension and the size factor. For example if widthFactor is
    /// 2.0 then the width of this widget will always be twice its child's width.
    struct Align {

        /// @brief How to align the child.
        /// @param value
        Align& alignment(const Alignment& value);

        /// @brief The widget below this widget in the tree.
        /// @tparam child_widget_t
        /// @param value
        template <typename child_widget_t>
        Align& child(child_widget_t& value)
        {
            adopt(this, value);
            return *this;
        }

        /// @brief Sets its height to the child's height multiplied by this factor.
        /// @param value
        Align& heightFactor(const float1 value);

        /// @brief Sets its width to the child's width multiplied by this factor.
        /// @param value
        Align& widthFactor(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        Alignment _alignment = Alignment::center();
        float1 _heightFactor = 1.f;
        float1 _widthFactor = 1.f;
    };

}
}