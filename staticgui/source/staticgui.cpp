//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <imgui.h>

#include <staticgui/staticgui.hpp>

namespace staticgui {

void throw_error(const std::string& what)
{
    detail::throw_user_bad_implementation(what);
}

#pragma region curve

curve::curve(const curve_preset preset)
{
    // switch on preset
    _data.spline = std::make_shared<detail::bspline>(0.f, 1.f, std::vector<float2> {});
}
curve::curve(const float departure, const float arrival, const std::vector<float2>& controls)
{
    _data.spline = std::make_shared<detail::bspline>(departure, arrival, controls);
}

curve::curve(const curve& other)
{
    *this = other;
}

curve& curve::operator=(const curve& other)
{
    _data = other._data;
    return *this;
}

curve::curve(curve&& other)
{
    *this = std::move(other);
}

curve& curve::operator=(curve&& other)
{
    _data = other._data;
    return *this;
}

#pragma endregion

draw_command::draw_command(detail::command_data& data)
    : _command_data(data)
{
}

draw_command::draw_command(const draw_command& other)
    : _command_data(other._command_data)
{
}

draw_command& draw_command::operator=(const draw_command& other)
{
    return *this;
}

draw_command::draw_command(draw_command&& other)
    : _command_data(other._command_data)
{
}

draw_command& draw_command::operator=(draw_command&& other)
{
    return *this;
}

void draw_command::draw_line(
    const float2& first_point, const float2& second_point,
    const float4& color,
    const float thickness)
{
    ImVec2 _first_point { first_point.x(), first_point.y() };
    ImVec2 _second_point { second_point.x(), second_point.y() };
    ImU32 _color = ImGui::GetColorU32({ color.x(), color.y(), color.z(), color.w() });
    _command_data.add_command([=](ImDrawList* _drawlist) {
        _drawlist->AddLine(_first_point, _second_point, _color, thickness);
    });
}

void draw_command::draw_rect(
    const float2& min_point, const float2& max_point,
    const float4& color,
    const float rounding,
    const float thickness)
{
    ImVec2 _min_point { min_point.x(), min_point.y() };
    ImVec2 _max_point { max_point.x(), max_point.y() };
    ImU32 _color = ImGui::GetColorU32({ color.x(), color.y(), color.z(), color.w() });
    _command_data.add_command([=](ImDrawList* _drawlist) {
        _drawlist->AddRect(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All, thickness);
    });
}

// layout::layout() { }

context::context() { }

context& get_context()
{
    static context _context;
    return _context;
}

context& context::max_fps(const unsigned int fps)
{
    detail::state.context.max_fps = fps;
    return *this;
}

}