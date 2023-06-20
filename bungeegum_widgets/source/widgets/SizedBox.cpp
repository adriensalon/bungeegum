#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/widgets/SizedBox.hpp>

namespace bungeegum {
namespace widgets {

    SizedBox& SizedBox::height(const float1 value)
    {
        _size.y = value;
        return *this;
    }

    SizedBox& SizedBox::width(const float1 value)
    {
        _size.x = value;
        return *this;
    }

    void SizedBox::resolve(resolve_command& command)
    {
        BoxConstraints _parentConstraints(command.min_size(), command.max_size());
        Size _constrainedSize = _parentConstraints.constrain(_size);
        if (_child.has_value()) {
            float2 _childSize = command.resolve_child(_child.value(), _constrainedSize, _constrainedSize);
            command.position_child(_child.value(), zero<float2>);
            command.resize(_childSize);
        } else {
            command.resize(_constrainedSize);
        }
    }

}
}