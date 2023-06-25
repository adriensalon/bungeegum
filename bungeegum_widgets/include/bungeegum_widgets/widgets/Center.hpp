#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that centers its child within itself.
    /// @details This widget will be as big as possible if its dimensions are constrained and
    /// widthFactor and heightFactor are null. If a dimension is unconstrained and the
    /// corresponding size factor is null then the widget will match its child's size in that
    /// dimension. If a size factor is non-null then the corresponding dimension of this widget
    /// will be the product of the child's dimension and the size factor. For example if
    /// widthFactor is 2.0 then the width of this widget will always be twice its child's width.
    struct Center {

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        Center& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        /// @brief Sets its height to the child's height multiplied by this factor.
        Center& heightFactor(const float1 value);

        /// @brief Sets its width to the child's width multiplied by this factor.
        Center& widthFactor(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        float2 _sizeFactor = { 1.f, 1.f };
    };

}
}