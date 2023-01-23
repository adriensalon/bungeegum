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
    template <typename child_widget_t>
    struct container {
        container(const child_widget_t& child)
        {
            // build(image());
        }

        void draw(const renderer& context)
        {
        }

    private:
        float _width_factor = 1.f;
        float _height_factor = 1.f;
    };

}
}