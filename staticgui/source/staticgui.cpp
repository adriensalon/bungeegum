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
    return *this;
}

draw_command& draw_command::add_rectangle(const draw_rectangle_command& rectangle_command)
{
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