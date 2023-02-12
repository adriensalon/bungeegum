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

#pragma region resolve

resolve_constraint::resolve_constraint(const float2& min_size, const float2& max_size)
{
}

resolve_constraint::resolve_constraint(const resolve_constraint& other)
{
}

resolve_constraint& resolve_constraint::operator=(const resolve_constraint& other)
{
    return *this;
}

resolve_constraint::resolve_constraint(resolve_constraint&& other)
{
}

resolve_constraint& resolve_constraint::operator=(resolve_constraint&& other)
{
    return *this;
}

float2& resolve_constraint::min_size()
{
    return _data.min_size;
}

const float2& resolve_constraint::min_size() const
{
    return _data.min_size;
}

float2& resolve_constraint::max_size()
{
    return _data.max_size;
}

const float2& resolve_constraint::max_size() const
{
    return _data.max_size;
}

resolve_constraint resolve_constraint::flip()
{
    return resolve_constraint(
        { min_size().y(), min_size().x() },
        { max_size().y(), max_size().x() });
}

resolve_constraint resolve_constraint::normalize()
{
    float _min_width = std::fmax(min_size().x(), 0.f);
    float _min_height = std::fmax(min_size().y(), 0.f);
    float _max_width = std::fmax(_min_width, max_size().x());
    float _max_height = std::fmax(_min_height, max_size().y());
    return resolve_constraint(
        { _min_width, _min_height },
        { _max_width, _max_height });
}

// resolve_constraint resolve_constraint::enforce(const resolve_constraint& constraint) {
//     minWidth : clampDouble(minWidth, constraints.minWidth, constraints.maxWidth),
//     maxWidth : clampDouble(maxWidth, constraints.minWidth, constraints.maxWidth),
//     minHeight : clampDouble(minHeight, constraints.minHeight, constraints.maxHeight),
//     maxHeight : clampDouble(maxHeight, constraints.minHeight, constraints.maxHeight),
// }

// resolve_constraint resolve_constraint::deflate(const float2& insets)
// {
// }

resolve_constraint resolve_constraint::loosen()
{
    return resolve_constraint(
        { 0.f, 0.f },
        max_size());
}

// resolve_constraint resolve_constraint::tighten(const float2& size)
// {
// }

bool resolve_constraint::has_bounded_height() const
{
    return (max_size().y() < infinity<float>);
}

bool resolve_constraint::has_bounded_width() const
{
    return (max_size().x() < infinity<float>);
}

bool resolve_constraint::has_infinite_height() const
{
    return (min_size().y() >= infinity<float>);
}

bool resolve_constraint::has_infinite_width() const
{
    return (min_size().x() >= infinity<float>);
}

bool resolve_constraint::has_tight_height() const
{
    return (min_size().y() >= max_size().y());
}

bool resolve_constraint::has_tight_width() const
{
    return (min_size().x() >= max_size().x());
}

bool resolve_constraint::is_normalized() const
{
    bool _width_normalized = min_size().x() >= 0.f && min_size().x() <= max_size().x();
    bool _height_normalized = min_size().y() >= 0.f && min_size().y() <= max_size().y();
    return (_width_normalized && _height_normalized);
}

// float2 resolve_constraint::constrain(const float2& size) const
// {
// }

// float2 resolve_constraint::biggest() const
// {
//     return constrain({ infinity<float>, infinity<float> });
// }

// float2 resolve_constraint::smallest() const
// {
//     return constrain({ 0.f, 0.f });
// }

#pragma endregion

draw_command::draw_command(detail::draw_command_data& data)
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