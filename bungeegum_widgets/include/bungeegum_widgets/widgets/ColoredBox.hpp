#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that paints its area with a specified Color and then draws its child on
    /// top of that color.
    struct ColoredBox {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        ColoredBox& child(child_widget_t& value)
        {
            if (_child.has_value())
                abandon(this, _child.value());
            _child = runtime_widget(value);
            adopt(this, _child.value());
            return *this;
        }

        /// @brief The color to paint the background area with.
        ColoredBox& color(const Color value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        void draw(draw_command& command);

        Color _color = { 0xFF000000 };
        std::optional<runtime_widget> _child = std::nullopt;
    };
}
}