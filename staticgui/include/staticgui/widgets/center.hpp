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
    template <typename child_widget_t>
    struct center : base_widget {

        auto create(build_context& context)
        {
            return image();
        }

        void update(build_context& context)
        {
            std::cout << "width factor = " << _width_factor << std::endl;
        }

        float _x;
        center(child_widget_t&& child)
            : _cr(build2(this))
        {

            // build_stateful(
            //     this, [&](build_context& context) { return child; },
            //     [&](build_context& context) { std::cout << "update" << std::endl; });
            // build_advanced(this, [this](build_advanced_context& context) {
            //     std::cout << "center\n";
            // });
        }

        // center(center&& other) noexcept
        // {
        //     // return *this;
        // }

        // center& operator=(center&& other) noexcept
        // {
        //     return *this;
        // }

        /// @brief
        /// @param factor
        /// @return
        center width_factor(const float factor)
        {
            _cr._width_factor = factor;
            return *this;
        }

        /// @brief
        /// @param factor
        /// @return
        center height_factor(const float factor)
        {
            _cr._height_factor = factor;
            return std::ref(*this);
        }

    private:
        center& _cr;
        float _width_factor = 1.f;
        float _height_factor = 1.f;
    };

}
}