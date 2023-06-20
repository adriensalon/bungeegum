#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/BoxConstraints.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that imposes additional constraints on its child.
    struct ConstrainedBox {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        ConstrainedBox& child(child_widget_t& value)
        {
            if (_child.has_value())
                abandon(this, _child.value());
            _child = adopt(this, value);
            return *this;
        }

        ConstrainedBox& constraints(const BoxConstraints value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<adopted_widget> _child = std::nullopt;
        BoxConstraints _constraints = BoxConstraints::expand(); // ig
    };

}
}