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

    struct container_widget {

        template <typename child_widget_t>
        container_widget(child_widget_t& child_widget)
        {
            declare(this, child_widget);
        }

        inline void resolve(const resolve_constraint& cst, resolve_advice& adv) { }

        void draw(draw_command& command)
        {
            draw_rectangle_command _rect;
            _rect.min_point({ 20.f, 20.f })
                .max_point({ 200.f, 200.f })
                .thickness(50.f)
                .color({ 0.33f, 0.33f, 0.89f, 1.f });
            command.add_rectangle(_rect);
            std::cout << "DRAWING CONTAINER \n";
        }

    private:
        float4 _color = { 0.f, 0.f, 0.f, 1.f };
    };
}

template <typename child_widget_t>
widgets::container_widget& container(child_widget_t& child_widget) { return make<widgets::container_widget>(child_widget); }

}