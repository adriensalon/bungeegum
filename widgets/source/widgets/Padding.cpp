#include <bungeegum_widgets/widgets/Padding.hpp>

namespace bungeegum {
namespace widgets {

    Padding& Padding::padding(const EdgeInsets value)
    {
        _edgeInsets = value;
        return *this;
    }

    void Padding::resolve(resolve_command& command)
    {
        if (_childWidget.has_value()) {
            float2 _padding = { _edgeInsets.horizontal(), _edgeInsets.vertical() };
            Offset _topLeft = _edgeInsets.topLeft();
            float2 _paddedPosition = { _topLeft.dx(), _topLeft.dy() };
            float2 _paddedMinSize = command.get_min_size() - _padding;
            float2 _paddedMaxSize = command.get_max_size() - _padding;
            float2 _childSize = command.resolve_child(_childWidget.value(), _paddedMinSize, _paddedMaxSize);
            command.position_child(_childWidget.value(), _paddedPosition);
            command.resize(_childSize + _padding);
        } else
            command.resize(command.get_max_size());
    }
}
}