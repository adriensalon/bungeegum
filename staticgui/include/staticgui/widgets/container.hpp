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
            command.draw_rect(
                { 20.f, 20.f },
                { 600.f, 8 * _tick_value },
                { _tick_value * 0.01f, 0.33f, 0.89f, 1.f },
                12,
                40);
            command.draw_rect(
                { 400.f, 400.f },
                { 500.f, 420 + _tick_value },
                { 0.81f, 0.53f, _tick_value * 0.0089f, 1.f },
                10,
                10);

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