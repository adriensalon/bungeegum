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
            draw_line_command _line;
            command.add_line(_line);
            command.add_line(_line);
            command.add_line(_line);
            std::cout << "DRAWING CONTAINER \n";
        }

    private:
        float4 _color = { 0.f, 0.f, 0.f, 1.f };
    };
}

template <typename child_widget_t>
widgets::container_widget& container(child_widget_t& child_widget) { return make<widgets::container_widget>(child_widget); }

}