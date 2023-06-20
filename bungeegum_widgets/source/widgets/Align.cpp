#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/widgets/Align.hpp>

namespace bungeegum {
namespace widgets {

    namespace {

        void _alignChildWidget(
            resolve_command& command,
            adopted_widget& childWidget,
            const float2 childSize,
            const float2 alignSize,
            const Alignment alignment)
        {
            (void)command;
            (void)childWidget;
            (void)childSize;
            (void)alignSize;
            (void)alignment;
            // TODO
        }

    }

    Align& Align::alignment(const Alignment& value)
    {
        _alignment = value;
        return *this;
    }

    Align& Align::heightFactor(const float1 value)
    {
        _sizeFactor.y = value;
        return *this;
    }

    Align& Align::widthFactor(const float1 value)
    {
        _sizeFactor.x = value;
        return *this;
    }

    void Align::resolve(resolve_command& command)
    {
        BoxConstraints _parentConstraints(command);
        Size _biggest = _parentConstraints.biggest();
        bool2 _hasConstrainedSize = {
            _parentConstraints.hasBoundedWidth() && (_sizeFactor.x == 1.f),
            _parentConstraints.hasBoundedHeight() && (_sizeFactor.y == 1.f)
        };
        if (_hasConstrainedSize.x && _hasConstrainedSize.y) {
            command.resize(_biggest);
            if (_child.has_value()) {
                float2 _childSize = command.resolve_child(_child.value(), zero<float2>, _biggest);
                _alignChildWidget(command, _child.value(), _childSize, _biggest, _alignment);
            }
        } else if (!_child.has_value()) {
            throw_error("Error TODO");
        } else {
            float2 _childSize = command.resolve_child(_child.value(), zero<float2>, _biggest);
            float2 _alignSize = zero<float2>;
            if (_hasConstrainedSize.x) {
                _alignSize.x = _biggest.width();
            } else {
                _alignSize.x = _sizeFactor.x * _childSize.x;
            }
            if (_hasConstrainedSize.y) {
                _alignSize.y = _biggest.height();
            } else {
                _alignSize.y = _sizeFactor.y * _childSize.y;
            }
            command.resize(_alignSize);
            _alignChildWidget(command, _child.value(), _childSize, _alignSize, _alignment);
        }
    }

}
}