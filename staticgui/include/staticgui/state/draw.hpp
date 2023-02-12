//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>
#include <vector>

#include <staticgui/glue/registry.hpp>
#include <staticgui/glue/simd.hpp>

struct ImDrawList;

namespace staticgui {
namespace detail {

    struct draw_command_data {
        void add_command(const std::function<void(ImDrawList*)>& commnad_callback);
        void draw();
        void draw(ImDrawList* imgui_drawlist);
        void clear();

        // private:
        std::vector<std::function<void(ImDrawList*)>> _commands;
    };

}
}
