#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A box that limits its size only when it's unconstrained.
    /// @details If this widget's maximum width is unconstrained then its child's width is limited
    /// to maxWidth. Similarly, if this widget's maximum height is unconstrained then its child's
    /// height is limited to maxHeight.
    /// This has the effect of giving the child a natural dimension in unbounded environments. For
    /// example, by providing a maxHeight to a widget that normally tries to be as big as possible,
    /// the widget will normally size itself to fit its parent, but when placed in a vertical list,
    /// it will take on the given height.
    /// This is useful when composing widgets that normally try to match their parents' size, so
    /// that they behave reasonably in lists (which are unbounded).
    struct LimitedBox {

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        LimitedBox& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        /// @brief The maximum height limit to apply in the absence of a maxHeight constraint.
        LimitedBox& maxHeight(const float1 value);

        /// @brief The maximum width limit to apply in the absence of a maxWidth constraint.
        LimitedBox& maxWidth(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        float2 _maxSize = infinity<float2>;
    };

}
}