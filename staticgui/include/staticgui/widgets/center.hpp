//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/staticgui.hpp>
#include <staticgui/widgets/image.hpp>

namespace staticgui {
namespace widgets {

    /// @brief A widget that centers its child within itself.
    /// @details This widget will be as big as possible if its dimensions are constrained and
    /// 'width_factor' and 'height_factor are not specified. If a dimension is unconstrained and the
    /// corresponding size factor is not specified then the widget will match its child's size in that
    /// dimension. If a size factor is specified then the corresponding dimension of this widget will
    /// be the product of the child's dimension and the size factor. For example if 'width_factor' is
    /// 2.0 then the width of this widget will always be twice its child's width.
    /// @tparam child_widget_t
    struct center_widget {

        template <typename child_widget_t>
        center_widget(child_widget_t& child)
        {
            std::cout << "ctor\n";
            // child_widget_t& ref;
            // auto& cc = image().set(88.f);
            // std::cout << cc.x << std::endl;
            build(this, child);
            // std::cout << cc.x << std::endl;
        }
        // center_widget& operator=(center_widget&& other)
        // {
        //     _width_factor = other._width_factor;
        //     return *this;
        // }

        // center_widget(center_widget&& other) noexcept
        // {
        //     *this = std::move(other);
        // }

        float ok()
        {
            return _width_factor;
        }

        ~center_widget()
        {
            std::cout << "center out\n";
        }

        float x = 0.f;

        center_widget& set(float xx)
        {
            x = xx;
            return *this;
        }

        /// @brief
        /// @param factor
        /// @return
        center_widget& width_factor(const float factor)
        {
            _width_factor = factor;
            return *this;
        }

        /// @brief
        /// @param factor
        /// @return
        center_widget& height_factor(const float factor)
        {
            std::cout << "previous = " << _height_factor << std::endl;
            _height_factor = factor;
            return *this;
        }

        constexpr static char* internal_name = "center";

    private:
        float _width_factor = 1.f;
        float _height_factor = 1.f;

        STATICGUI_WIDGET(center_widget)
    };

}
}

#define center(child_widget) staticgui::make<staticgui::widgets::center_widget>(child_widget)

// template <typename child_widget_t>
// using center = staticgui::make<staticgui::widgets::center_widget, child_widget_t>;