#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/BoxConstraints.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that imposes additional constraints on its child.
    /// @details For example, if you wanted child to have a minimum height of 50.f logical pixels,
    /// you could use make<BoxConstraints>().minHeight(50.f) as the constraints.
    struct ConstrainedBox {

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        ConstrainedBox& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        /// @brief The additional constraints to impose on the child.
        ConstrainedBox& constraints(const BoxConstraints value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        BoxConstraints _constraints = BoxConstraints::expand(); // ig
    };

}
}