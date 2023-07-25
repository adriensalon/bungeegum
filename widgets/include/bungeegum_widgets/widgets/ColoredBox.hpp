#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    struct mydata {

        std::string _mystr = "okok";

        // HOTSWAP_CLASS(mydata, _mystr)
        SERIALIZE_FIELDS(_mystr)

        // template <typename archive_t>
        // void serialize(archive_t& archive)
        // {
        //     archive(_mystr);
        // }
    };

    /// @brief A widget that paints its area with a specified Color and then draws its child on
    /// top of that color.
    struct ColoredBox {

        inline ColoredBox()
        {
            std::cout << " create ColorBox \n";
        }

        inline ColoredBox(int ok)
        {
            std::cout << ok << " create ColorBox \n";
        }

        /// @brief The widget below this widget in the tree.
        // template <typename childWidget_t>
        // ColoredBox& child(childWidget_t& value)
        // {
        //     if (_childWidget.has_value())
        //         abandon(this, _childWidget.value());
        //     _childWidget = runtime_widget(value);
        //     adopt(this, _childWidget.value());
        //     return *this;
        // }

        ColoredBox& child(const runtime_widget& value);

        /// @brief The color to paint the background area with.
        ColoredBox& color(const Color value);

    private:
        friend struct access;
        HOTSWAP_METHOD void resolve(resolve_command& command);
        HOTSWAP_METHOD void draw(draw_command& command);

        Color _color = { 0xFF000000 };
        std::optional<runtime_widget> _childWidget = std::nullopt;

        int ii = 43;
        float ff = 56.f;
        float ff2 = 56.f;
        float ff3 = 56.f;
        float ff4 = 56.f;
        float ff44 = 56.f;
        mydata _md;

        HOTSWAP_CLASS(ColoredBox, ii, ff, ff2, ff3, _md, _color)
    };
}
}