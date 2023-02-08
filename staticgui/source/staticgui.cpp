//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

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
    _data.spline = std::make_shared<glue::bspline>(0.f, 1.f, std::vector<float2> {});
}
curve::curve(const float departure, const float arrival, const std::vector<float2>& controls)
{
    _data.spline = std::make_shared<glue::bspline>(departure, arrival, controls);
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

draw_rounding_command& draw_rounding_command::strength(const float z)
{
    _rounding.strength = z;
    return *this;
}

draw_rounding_command& draw_rounding_command::top_left(const bool enable)
{
    _rounding.top_left = enable;
    return *this;
}

draw_rounding_command& draw_rounding_command::top_right(const bool enable)
{

    _rounding.top_right = enable;
    return *this;
}

draw_rounding_command& draw_rounding_command::bottom_left(const bool enable)
{

    _rounding.bottom_left = enable;
    return *this;
}

draw_rounding_command& draw_rounding_command::bottom_right(const bool enable)
{

    _rounding.bottom_right = enable;
    return *this;
}

draw_rectangle_command& draw_rectangle_command::min_point(const float2& first)
{
    _rectangle_command_data.min_point = first;
    return *this;
}

draw_rectangle_command& draw_rectangle_command::max_point(const float2& second)
{
    _rectangle_command_data.max_point = second;
    return *this;
}

draw_rectangle_command& draw_rectangle_command::color(const float4& col)
{
    _rectangle_command_data.color = col;
    return *this;
} // go color !!

draw_rectangle_command& draw_rectangle_command::rounding(const draw_rounding_command& rounding_command)
{
    _rectangle_command_data.rounding = rounding_command._rounding;
    return *this;
}

draw_rectangle_command& draw_rectangle_command::thickness(const float t)
{
    _rectangle_command_data.thickness = t;
    return *this;
}

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

draw_command& draw_command::add_line(const draw_line_command& line_command)
{
    _command_data.add_line(line_command._line_command_data);
    return *this;
}

draw_command& draw_command::add_rectangle(const draw_rectangle_command& rectangle_command)
{
    this->_command_data.add_rectangle(rectangle_command._rectangle_command_data);
    return *this;
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