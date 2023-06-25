
#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/widgets/LimitedBox.hpp>

namespace bungeegum {
namespace widgets {

    LimitedBox& LimitedBox::maxHeight(const float1 value)
    {
        _maxSize.y = value;
        return *this;
    }

    LimitedBox& LimitedBox::maxWidth(const float1 value)
    {
        _maxSize.x = value;
        return *this;
    }

    void LimitedBox::resolve(resolve_command& command)
    {
        BoxConstraints _parentConstraints(command.min_size(), command.max_size());
        if (_childWidget.has_value()) {
            float2 _childMaxSize = _parentConstraints.biggest();
            if (!_parentConstraints.hasBoundedWidth()) {
                _childMaxSize.x = _parentConstraints.constrainWidth(_maxSize.x);
            }
            if (!_parentConstraints.hasBoundedHeight()) {
                _childMaxSize.y = _parentConstraints.constrainHeight(_maxSize.y);
            }
            float2 _childSize = command.resolve_child(_childWidget.value(), _parentConstraints.smallest(), _childMaxSize);
            command.position_child(_childWidget.value(), zero<float2>);
            command.resize(_childSize);
        } else {
            command.resize(_parentConstraints.smallest());
        }
    }

}
}