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

    struct line_command_data {
        glue::simd_array<float, 2> first_point;
        glue::simd_array<float, 2> second_point;
        glue::simd_array<float, 4> color;
        float thickness = 1.f;
    };

    struct rect_command_data {
        glue::simd_array<float, 2> min_point;
        glue::simd_array<float, 2> max_point;
        glue::simd_array<float, 4> color;
        float rounding = 0.f;
        float thickness = 1.f;
    };

    struct command_data {
        void add_command(const std::function<void(ImDrawList*)>& commnad_callback);
        void draw();
        void draw(ImDrawList* imgui_drawlist);
        void clear();

    private:
        // std::any _command_impl = nullptr;
        std::vector<std::function<void(ImDrawList*)>> _commands;
    };

    using draw_function = std::function<void(command_data&)>;
}
}
