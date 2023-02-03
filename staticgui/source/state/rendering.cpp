//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <imgui.h>

#include <staticgui/state/rendering.hpp>

namespace staticgui {
namespace detail {

    ImVec2 get_vec2(const glue::simd_array<float, 2>& vec2)
    {
        float _x, _y;
        _x = vec2.x();
        _y = vec2.y();
        return ImVec2(_x, _y);
    }

    ImVec4 get_vec4(const glue::simd_array<float, 4>& vec4)
    {
        float _x, _y, _z, _w;
        _x = vec4.x();
        _y = vec4.y();
        _z = vec4.z();
        _w = vec4.w();
        return ImVec4(_x, _y, _z, _w);
    }

    ImU32 get_color(const glue::simd_array<float, 4>& color)
    {
        ImVec4 _imvec(get_vec4(color));
        return ImGui::GetColorU32(_imvec);
    }

    ImDrawFlags get_rounding(const rounding_data& data)
    {
        return ImDrawFlags_None;
    }

    void command_data::add_line(const line_command_data& line)
    {
        _lines.emplace_back(line);
    }

    void command_data::add_rectangle(const rectangle_command_data& rectangle)
    {
        _rectangles.emplace_back(rectangle);
    }

    void command_data::draw()
    {
        // if (ImGui::Begin("hello")) {
        //     ImGui::Text("Ok...");
        // ImDrawList _draw_list(ImGui::GetDrawListSharedData());
        ImDrawList* _draw_list = ImGui::GetForegroundDrawList();
        for (auto& _line : _lines) {
            ImVec2 _first_point = get_vec2(_line.first_point);
            ImVec2 _second_point = get_vec2(_line.second_point);
            ImU32 _color = get_color(_line.color);
            _draw_list->AddLine(_first_point, _second_point, _color, _line.thickness);
        }
        for (auto& _rectangle : _rectangles) {
            ImVec2 _min_point = get_vec2(_rectangle.min_point);
            ImVec2 _max_point = get_vec2(_rectangle.max_point);
            ImU32 _color = get_color(_rectangle.color);
            ImDrawFlags _flags = get_rounding(_rectangle.rounding);
            _draw_list->AddRectFilled(ImVec2(20.f, 20.f), ImVec2(200.f, 200.f), ImGui::GetColorU32(ImVec4(0.33f, 0.22f, 0.66f, 1.f)));
            // _draw_list.AddRectFilled(_min_point, _max_point, _color, _rectangle.rounding.strength, 0);
            // _draw_list.AddRect(_min_point, _max_point, _color, _rectangle.rounding.strength, _flags, _rectangle.thickness);
        }

        // ImGui::End();
        // }
    }

    void command_data::clear()
    {
        _lines.clear();
        _rectangles.clear();
    }

}
}