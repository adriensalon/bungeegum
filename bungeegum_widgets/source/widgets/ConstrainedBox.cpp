#include <bungeegum_widgets/widgets/ConstrainedBox.hpp>

namespace bungeegum {
namespace widgets {

    ConstrainedBox& ConstrainedBox::constraints(const BoxConstraints value)
    {
        _constraints = value;
        return *this;
    }

    void ConstrainedBox::resolve(resolve_command& command)
    {
        if (_child.has_value()) {
            float2 _childSize = command.resolve_child(
                _child.value(),
                _constraints.constrain(command.min_size()),
                _constraints.constrain(command.max_size()));
            command.resize(_childSize);
        } else {
            command.resize(_constraints.smallest());
        }
    }

}
}