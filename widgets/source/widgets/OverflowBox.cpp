#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/widgets/OverflowBox.hpp>

namespace bungeegum {
namespace widgets {

    namespace {

        void _alignChildWidget(
            resolve_command& command,
            runtime_widget& childWidget,
            const float2 childSize,
            const float2 overflowSize,
            const Alignment alignment)
        {
            float2 _alignment = static_cast<float2>(alignment);
            float2 _clampedAlignment = { glm::clamp(_alignment.x, -1.f, 1.f), glm::clamp(_alignment.y, -1.f, 1.f) };
            float2 _normalizedAlignment = 0.5f * (_clampedAlignment + float2 { 1.f });
            float2 _positionForChild = {
                _normalizedAlignment.x * childSize.x,
                _normalizedAlignment.y * childSize.y
            };
            float2 _positionForOverflow = {
                _normalizedAlignment.x * overflowSize.x,
                _normalizedAlignment.y * overflowSize.y
            };
            float2 _positionDelta = _positionForOverflow - _positionForChild;
            command.position_child(childWidget, _positionDelta);
        }

    }

    OverflowBox& OverflowBox::alignment(const Alignment value)
    {
        _alignment = value;
        return *this;
    }

    OverflowBox& OverflowBox::child(const runtime_widget& value)
    {
        if (_childWidget.has_value())
            abandon(this, _childWidget.value());
        _childWidget = value;
        adopt(this, _childWidget.value());
        return *this;
    }

    OverflowBox& OverflowBox::maxHeight(const std::optional<float1> value)
    {
        _maxHeight = value;
        return *this;
    }

    OverflowBox& OverflowBox::maxWidth(const std::optional<float1> value)
    {
        _maxWidth = value;
        return *this;
    }

    OverflowBox& OverflowBox::minHeight(const std::optional<float1> value)
    {
        _minHeight = value;
        return *this;
    }

    OverflowBox& OverflowBox::minWidth(const std::optional<float1> value)
    {
        _minWidth = value;
        return *this;
    }

    void OverflowBox::resolve(resolve_command& command)
    {
        BoxConstraints _parentConstraints(command);
        float2 _minSize = _parentConstraints.smallest();
        float2 _maxSize = _parentConstraints.biggest();
        float2 _overflowSize = _maxSize;
        command.resize(_overflowSize);
        if (_maxHeight.has_value()) {
            _maxSize.y = _maxHeight.value();
        }
        if (_maxWidth.has_value()) {
            _maxSize.x = _maxWidth.value();
        }
        if (_childWidget.has_value()) {
            if (_minHeight.has_value()) {
                _minSize.y = _minHeight.value();
            }
            if (_minWidth.has_value()) {
                _minSize.x = _minWidth.value();
            }
            float2 _childSize = command.resolve_child(_childWidget.value(), _minSize, _maxSize);
            _alignChildWidget(command, _childWidget.value(), _childSize, _overflowSize, _alignment);
        }
    }

}
}