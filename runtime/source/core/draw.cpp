#include <ImGui.h>

#include <bungeegum/core/draw.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/widget.hpp>

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

float2 draw_command::resolved_position() const
{
    return _data.resolved_position;
}

float2 draw_command::resolved_size() const
{
    return _data.resolved_size;
}

// void draw_command::clip_rect(const float2 first_point, const float2 second_point)
// {
//     _data.clipping = {
//         first_point,
//         second_point,
//         true
//     };
// #if BUNGEEGUM_USE_OVERLAY
//     std::pair<std::string, std::vector<std::string>>& _info = _data.commands_infos.emplace_back();
//     _info.first = "clip rect";
//     _info.second.push_back(to_string<float2>(first_point));
//     _info.second.push_back(to_string<float2>(second_point));
// #endif
// }

void draw_command::draw_line(
    const float2 first_point, const float2 second_point,
    const float4 color,
    const float thickness)
{
    ImVec2 _first_point { first_point.x, first_point.y };
    ImVec2 _second_point { second_point.x, second_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.commands.emplace_back([=](ImDrawList* _drawlist) {
        _drawlist->AddLine(_first_point, _second_point, _color, thickness);
    });
#if BUNGEEGUM_USE_OVERLAY
    std::pair<std::string, std::vector<std::string>>& _info = _data.commands_infos.emplace_back();
    _info.first = "draw line";
    _info.second.push_back(to_string<float2>(first_point));
    _info.second.push_back(to_string<float2>(second_point));
    _info.second.push_back(to_string<float4>(color));
    _info.second.push_back(to_string<float1>(thickness));
#endif
}

void draw_command::draw_rect(
    const float2 min_point, const float2 max_point,
    const float4 color,
    const float rounding,
    const float thickness)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.commands.emplace_back([=](ImDrawList* _drawlist) {
        _drawlist->AddRect(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All, thickness);
    });
#if BUNGEEGUM_USE_OVERLAY
    std::pair<std::string, std::vector<std::string>>& _info = _data.commands_infos.emplace_back();
    _info.first = "draw rect";
    _info.second.push_back(to_string<float2>(min_point));
    _info.second.push_back(to_string<float2>(max_point));
    _info.second.push_back(to_string<float4>(color));
    _info.second.push_back(to_string<float1>(rounding));
    _info.second.push_back(to_string<float1>(thickness));
#endif
}

void draw_command::draw_rect_filled(
    const float2 min_point, const float2 max_point,
    const float4 color,
    const float rounding)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.commands.emplace_back([=](ImDrawList* _drawlist) {
        _drawlist->AddRectFilled(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All);
    });
#if BUNGEEGUM_USE_OVERLAY
    std::pair<std::string, std::vector<std::string>>& _info = _data.commands_infos.emplace_back();
    _info.first = "draw rect filled";
    _info.second.push_back(to_string<float2>(min_point));
    _info.second.push_back(to_string<float2>(max_point));
    _info.second.push_back(to_string<float4>(color));
    _info.second.push_back(to_string<float1>(rounding));
#endif
}

void must_draw()
{
    const std::uintptr_t _root = detail::global().widgets.root();
    detail::global().widgets.drawables.push_back(_root);
}
}