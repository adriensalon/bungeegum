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

}