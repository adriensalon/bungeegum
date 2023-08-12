#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/widgets/Align.hpp>

namespace bungeegum {
namespace widgets {

    namespace {

        void _alignChildWidget(
            resolve_command& command,
            runtime_widget& childWidget,
            const float2 childSize,
            const float2 alignSize,
            const Alignment alignment)
        {
            float2 _alignment = static_cast<float2>(alignment);
            float2 _clampedAlignment = { glm::clamp(_alignment.x, -1.f, 1.f), glm::clamp(_alignment.y, -1.f, 1.f) };
            float2 _normalizedAlignment = 0.5f * (_clampedAlignment + float2 { 1.f });
            float2 _positionForChild = {
                _normalizedAlignment.x * childSize.x,
                _normalizedAlignment.y * childSize.y
            };
            float2 _positionForAlign = {
                _normalizedAlignment.x * alignSize.x,
                _normalizedAlignment.y * alignSize.y
            };
            float2 _positionDelta = _positionForAlign - _positionForChild;
            command.position_child(childWidget, _positionDelta);
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
            if (_childWidget.has_value()) {
                float2 _childSize = command.resolve_child(_childWidget.value(), zero<float2>, _biggest);
                _alignChildWidget(command, _childWidget.value(), _childSize, _biggest, _alignment);
            }
        } else if (!_childWidget.has_value()) {
            log_error("Error TODO");
        } else {
            // je pense qu'il faut avoir le parent de la taille biggest
            // et ajuster plutot la taille du child avec les if else
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
            _alignChildWidget(command, _childWidget.value(), _childSize, _alignSize, _alignment);
        }
    }

}
}