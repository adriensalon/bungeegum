#include <bungeegum/widgets/Padding.hpp>

namespace bungeegum {
namespace widgets {

    Padding& Padding::padding(const EdgeInsets& value)
    {
        _edgeInsets = value;
        return *this;
    }

    Padding& Padding::padding(const EdgeInsetsDirectional& value)
    {
        _edgeInsets = value;
        return *this;
    }

    void Padding::resolve(resolve_command& command)
    {
        // TODO :::::===))))

        if (_child.has_value()) {
            float2 _childSize = command.resolve_child(_child.value(), command.min_size(), command.max_size());
            command.position_child(_child.value(), zero<float2>);
            command.resize(_childSize);
        } else
            command.resize(command.max_size());
    }
}
}