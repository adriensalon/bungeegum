//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <cstdint>

#include <staticgui/staticgui.hpp>

namespace staticgui {
namespace widgets {

    struct container_widget {

        template <typename child_widget_t>
        container_widget(child_widget_t& child_widget, const animatable<float>& testanim)
        {
            declare(this, child_widget);
            testanim.assign(this, _tick_value);
        }

        inline void resolve(const resolve_constraint& cst, resolve_advice& adv) { }

        void draw(draw_command& command)
        {
            draw_rounding_command _rounding;
            _rounding.strength(20);
            draw_rectangle_command _rect;
            _rect.min_point({ 20.f, 20.f })
                .max_point({ 600.f, _tick_value })
                .thickness(40)
                .color({ _tick_value * 0.01f, 0.33f, 0.89f, 1.f })
                .rounding(_rounding);
            command.add_rectangle(_rect);
            // std::cout << "DRAWING CONTAINER \n";
            // detail::throw_library_bad_implementation("helloww");
            // detail::throw_library_bad_usage("helloww");
            // throw_error("my user error");
        }

    private:
        float _tick_value = 20.f;
    };
}

template <typename child_widget_t>
widgets::container_widget& container(child_widget_t& child_widget, const animatable<float>& testanim) { return make<widgets::container_widget>(child_widget, testanim); }

}