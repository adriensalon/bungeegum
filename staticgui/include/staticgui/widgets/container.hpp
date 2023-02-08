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
        container_widget(child_widget_t& child_widget)
        {
            declare(this, child_widget);
            // _animpos.start()
            static bool setup = false;
            if (!setup) {
                static animation<float> _animpos(curve(0.f, 1.f), 10.f, 100.f, 3);
                _animpos.on_tick([&](const float& _tick) {
                    context& _ctx = get_context();
                    // must redraw !
                    // std::cout << "tick = " << _tick_value << std::endl;
                    std::cout << "me 1 = " << reinterpret_cast<uintptr_t>(this) << std::endl;
                    this->_tick_value = _tick;
                    _ctx.must_draw(this);
                });
                _animpos.start().detach();
                setup = true;
            }
        }

        inline void resolve(const resolve_constraint& cst, resolve_advice& adv) { }

        void draw(draw_command& command)
        {

            draw_rounding_command _rounding;
            _rounding.strength(_tick_value);
            draw_rectangle_command _rect;
            _rect.min_point(std::array<float, 2> { 20.f, 20.f })
                .max_point(std::array<float, 2> { 600.f, _tick_value })
                .thickness(_tick_value)
                .color(std::array<float, 4> { 0.33f, 0.33f, 0.89f, 1.f })
                .rounding(_rounding);
            command.add_rectangle(_rect);
            // std::cout << "DRAWING CONTAINER \n";
            // detail::throw_library_bad_implementation("helloww");
            // detail::throw_library_bad_usage("helloww");
            // throw_error("my user error");
            // std::cout << "_tick_value = " << _tick_value << std::endl;
            std::cout << "me 2 = " << reinterpret_cast<uintptr_t>(this) << std::endl;
        }

        ~container_widget()
        {
            std::cout << "DTPR CP?TAINER \n";
        }

    private:
        float _tick_value = 20.f;
    };
}

template <typename child_widget_t>
widgets::container_widget& container(child_widget_t& child_widget) { return make<widgets::container_widget>(child_widget); }

}