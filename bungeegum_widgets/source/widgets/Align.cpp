#include <bungeegum_widgets/misc/Alignment.hpp>
#include <bungeegum_widgets/widgets/Align.hpp>


namespace bungeegum {
namespace widgets {

    Align& Align::alignment(const Alignment& value)
    {
        _alignment = value;
        return *this;
    }

    Align& Align::heightFactor(const float1 value)
    {
        _heightFactor = value;
        return *this;
    }

    Align& Align::widthFactor(const float1 value)
    {
        _widthFactor = value;
        return *this;
    }

    void Align::resolve(resolve_command& command)
    {
        (void)command;
        // todo
        // command.position_child()
    }

}
}