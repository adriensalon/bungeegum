#include <bungeegum/widgets/ColoredBox.hpp>

namespace bungeegum {
namespace widgets {

    ColoredBox& ColoredBox::color(const Color value)
    {
        _color = value;
        return *this;
    }

    void ColoredBox::resolve(resolve_command& command)
    {
        if (_child.has_value()) {
            float2 _childSize = command.resolve_child(_child, command.min_size(), command.max_size());
            command.position_child(_child, zero<float2>);
            command.resize(_childSize);
        } else
            command.resize(command.max_size());
    }

    void ColoredBox::draw(draw_command& command)
    {
        float2 _min_point = command.resolved_position();
        float2 _max_point = _min_point + command.resolved_size();
        command.draw_rect_filled(_min_point, _max_point, _color);
    }

}
}