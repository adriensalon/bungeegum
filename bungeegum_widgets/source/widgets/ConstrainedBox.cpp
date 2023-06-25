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
        BoxConstraints _parentConstraints(command.min_size(), command.max_size());
        Size _constrainedMinSize = _parentConstraints.constrain(_constraints.smallest());
        if (_childWidget.has_value()) {
            Size _constrainedMaxSize = _parentConstraints.constrain(_constraints.biggest());
            float2 _childSize = command.resolve_child(_childWidget.value(), _constrainedMinSize, _constrainedMaxSize);
            command.position_child(_childWidget.value(), zero<float2>);
            command.resize(_childSize);
        } else {
            command.resize(_constrainedMinSize);
        }
    }

}
}