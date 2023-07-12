#include <bungeegum_widgets/widgets/CustomSingleChildLayout.hpp>

namespace bungeegum {
namespace widgets {

    void CustomSingleChildLayout::resolve(resolve_command& command)
    {
        if (!_delegate.untyped.has_value()) {
            log_error("Error");
        }
        if (!_childWidget.has_value()) {
            log_error("Error");
        }
        BoxConstraints _constraints(command);
        Size _size = _delegate.getSize(_constraints);
        command.resize(_size);
        BoxConstraints _childConstraints = _delegate.getConstraintsForChild(_constraints);
        Size _childSize = Size(command.resolve_child(
            _childWidget.value(),
            _childConstraints.smallest(),
            _childConstraints.biggest()));
        Offset _childPosition = _delegate.getPositionForChild(_size, _childSize);
        command.position_child(_childWidget.value(), _childPosition);
    }

}
}