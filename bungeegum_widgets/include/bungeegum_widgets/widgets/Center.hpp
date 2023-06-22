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
        /// @tparam child_widget_t
        /// @param value
        template <typename child_widget_t>
        Center& child(child_widget_t& value)
        {
            if (_child.has_value())
                abandon(this, _child.value());
            _child = runtime_widget(value);
            adopt(this, _child.value());
            return *this;
        }

        /// @brief Sets its height to the child's height multiplied by this factor.
        /// @param value
        Center& heightFactor(const float1 value);

        /// @brief Sets its width to the child's width multiplied by this factor.
        /// @param value
        Center& widthFactor(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _child = std::nullopt;
        float2 _sizeFactor = { 1.f, 1.f };
    };

}
}