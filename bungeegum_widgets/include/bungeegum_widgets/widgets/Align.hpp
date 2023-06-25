#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that aligns its child within itself and optionally sizes itself based on
    /// the child's size. For example, to align a box at the bottom right, you would pass this box
    /// a tight constraint that is bigger than the child's natural size, with an alignment of
    /// Alignment::bottomRight()
    /// @details This widget will be as big as possible if its dimensions are constrained and
    /// widthFactor and heightFactor are 1.f or unmodified. If a dimension is unconstrained the
    /// corresponding dimension of this widget will be the product of the child's dimension and
    /// the size factor. For example if widthFactor is 2.f then the width of this widget will
    /// always be twice its child's width.
    struct Align {

        /// @brief How to align the child.
        Align& alignment(const Alignment& value);

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        Align& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        /// @brief Sets its height to the child's height multiplied by this factor.
        Align& heightFactor(const float1 value);

        /// @brief Sets its width to the child's width multiplied by this factor.
        Align& widthFactor(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        Alignment _alignment = Alignment::center();
        float2 _sizeFactor = { 1.f, 1.f };
    };

}
}