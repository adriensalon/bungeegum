#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/misc/EdgeInsets.hpp>
#include <bungeegum/widgets/misc/EdgeInsetsDirectional.hpp>

namespace bungeegum {
namespace widgets {

    struct Padding {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        Padding& child(child_widget_t& value)
        {
            _child = adopt(this, value);
            return *this;
        }

        /// @brief The amount of space by which to inset the child.
        Padding& padding(const EdgeInsets& value);

        /// @brief The amount of space by which to inset the child.
        Padding& padding(const EdgeInsetsDirectional& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<adopted_widget> _child = std::nullopt;
        std::optional<std::variant<EdgeInsets, EdgeInsetsDirectional>> _edgeInsets = std::nullopt;
    };

}
}