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

    // rectangle command etc
    // text command etc
    // image command etc

    struct rounding_data {
        float strength = 0.f;
        bool top_left = true;
        bool top_right = true;
        bool bottom_left = true;
        bool bottom_right = true;
    };

    struct line_command_data {
        glue::simd_array<float, 2> first_point;
        glue::simd_array<float, 2> second_point;
        glue::simd_array<float, 4> color;
        float thickness = 1.f;
    };

    struct rectangle_command_data {
        glue::simd_array<float, 2> min_point;
        glue::simd_array<float, 2> max_point;
        glue::simd_array<float, 4> color;
        rounding_data rounding;
        float thickness = 1.f;
    };

    struct command_data {
        void add_line(const line_command_data& line);
        void add_rectangle(const rectangle_command_data& rectangle);
        void draw();
        void draw(ImDrawList* imgui_drawlist);
        void clear();

    private:
        // std::any _command_impl = nullptr;
        std::vector<line_command_data> _lines;
        std::vector<rectangle_command_data> _rectangles;
    };

    using draw_function = std::function<void(command_data&)>;
}
}
