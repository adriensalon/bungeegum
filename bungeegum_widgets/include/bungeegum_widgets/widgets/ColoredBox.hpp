#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that paints its area with a specified Color and then draws its child on
    /// top of that color.
    struct ColoredBox {

        inline ColoredBox()
        {
            std::cout << "create ColorBox \n";
        }

        HOTRELOAD_CLASS(ColoredBox, "ColoredBoxClass");

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        ColoredBox& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        /// @brief The color to paint the background area with.
        ColoredBox& color(const Color value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        HOTRELOAD_METHOD void draw(draw_command& command);

        Color _color = { 0xFF000000 };
        std::optional<runtime_widget> _childWidget = std::nullopt;

        int ii = 43;
        float ff = 3.33f;

        HOTRELOAD_FIELDS(ii, ff)
    };
}
}