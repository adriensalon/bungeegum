#include <bungeegum/widgets/Container.hpp>

namespace bungeegum {
namespace widgets {

    Container& Container::color(const Color value)
    {
        _color = value;
        return *this;
    }

    // Align& Align::heightFactor(const float1 value)
    // {
    //     _heightFactor = value;
    //     return *this;
    // }

    // Align& Align::widthFactor(const float1 value)
    // {
    //     _widthFactor = value;
    //     return *this;
    // }

    void Container::resolve(resolve_command& command)
    {
        (void)command;
        // todo
        // command.position_child()
    }

    void Container::draw(draw_command& command)
    {
        command.draw_rect(
            { 20.f, 20.f },
            { 600.f, 8.f },
            _color,
            12,
            40);
    }

}
}