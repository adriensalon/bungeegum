#include <bungeegum/widgets/ConstrainedBox.hpp>

namespace bungeegum {
namespace widgets {

    ConstrainedBox& ConstrainedBox::constraints(const BoxConstraints value)
    {
        _constraints = value;
        return *this;
    }

    void ConstrainedBox::resolve(resolve_command& command)
    {
        (void)command;
        // todo

        if (_child.has_value()) {
            float2 _childSize = command.resolve_child(_child.value(), _constraints._minSize, _constraints._maxSize);
            command.resize(_childSize);
        } else {
            command.resize(_constraints.smallest());
        }
    }

}
}