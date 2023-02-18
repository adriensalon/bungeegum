//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/image.hpp>

namespace bungeegum {
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
            build(this, child);
        }

        /// @brief
        /// @param factor
        /// @return
        center_widget& width_factor(const float& factor)
        {
            // factor.assign(_width_factor);
            return *this;
        }

        /// @brief
        /// @param factor
        /// @return
        center_widget& height_factor(const float& factor)
        {
            // factor.assign(_height_factor);
            return *this;
        }

    private:
        float _width_factor = 1.f;
        float _height_factor = 1.f;
    };

}
}

#define center bungeegum::create<bungeegum::widgets::center_widget>
