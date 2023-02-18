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

#include <bungeegum/context/draw.hpp>

namespace bungeegum {
namespace detail {

    void draw_command_data::draw()
    {
        ImDrawList* _draw_list = ImGui::GetBackgroundDrawList();
        draw(_draw_list);
    }

    void draw_command_data::draw(ImDrawList* imgui_drawlist)
    {
        for (auto& _command : commands)
            _command(imgui_drawlist);
    }

}

draw_command::draw_command(detail::draw_command_data& data)
    : _data(data)
{
}

draw_command::draw_command(const draw_command& other)
    : _data(other._data)
{
}

draw_command& draw_command::operator=(const draw_command& other)
{
    return *this;
}

draw_command::draw_command(draw_command&& other)
    : _data(other._data)
{
}

draw_command& draw_command::operator=(draw_command&& other)
{
    return *this;
}

void draw_command::draw_line(
    const float2 first_point, const float2 second_point,
    const float4 color,
    const float thickness)
{
    ImVec2 _first_point { first_point.x, first_point.y };
    ImVec2 _second_point { second_point.x, second_point.y };
    ImU32 _color = ImGui::GetColorU32({ color.x, color.y, color.z, color.w });
    _data.commands.emplace_back([=](ImDrawList* _drawlist) {
        _drawlist->AddLine(_first_point, _second_point, _color, thickness);
    });
}

void draw_command::draw_rect(
    const float2 min_point, const float2 max_point,
    const float4 color,
    const float rounding,
    const float thickness)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color = ImGui::GetColorU32({ color.x, color.y, color.z, color.w });
    _data.commands.emplace_back([=](ImDrawList* _drawlist) {
        _drawlist->AddRect(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All, thickness);
    });
}
}