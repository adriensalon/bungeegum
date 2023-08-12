#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/widgets/Center.hpp>

namespace bungeegum {
namespace widgets {

    namespace {

        void _centerChildWidget(
            resolve_command& command,
            runtime_widget& childWidget,
            const float2 childSize,
            const float2 centerSize)
        {
            float2 _positionForChild = {
                0.5f * childSize.x,
                0.5f * childSize.y
            };
            float2 _positionForAlign = {
                0.5f * centerSize.x,
                0.5f * centerSize.y
            };
            float2 _positionDelta = _positionForAlign - _positionForChild;
            command.position_child(childWidget, _positionDelta);
        }

    }

    Center& Center::heightFactor(const float1 value)
    {
        _sizeFactor.y = value;
        return *this;
    }

    Center& Center::widthFactor(const float1 value)
    {
        _sizeFactor.x = value;
        return *this;
    }

    void Center::resolve(resolve_command& command)
    {
        BoxConstraints _parentConstraints(command);
        Size _biggest = _parentConstraints.biggest();
        bool2 _hasConstrainedSize = {
            _parentConstraints.hasBoundedWidth() && (_sizeFactor.x == 1.f),
            _parentConstraints.hasBoundedHeight() && (_sizeFactor.y == 1.f)
        };
        if (_hasConstrainedSize.x && _hasConstrainedSize.y) {
            command.resize(_biggest);
            if (_childWidget.has_value()) {
                float2 _childSize = command.resolve_child(_childWidget.value(), zero<float2>, _biggest);
                _centerChildWidget(command, _childWidget.value(), _childSize, _biggest);
            }
        } else if (!_childWidget.has_value()) {
            log_error("Error TODO");
        } else {
            float2 _childSize = command.resolve_child(_childWidget.value(), zero<float2>, _biggest);
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
            _centerChildWidget(command, _childWidget.value(), _childSize, _alignSize);
        }
    }

}
}