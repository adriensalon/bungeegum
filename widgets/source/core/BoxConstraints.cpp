#include <bungeegum_widgets/core/BoxConstraints.hpp>

namespace bungeegum {
namespace widgets {

    BoxConstraints::BoxConstraints(const float minWidth, const float maxWidth, const float minHeight, const float maxHeight)
    {
        _minSize = float2 { minWidth, minHeight };
        _maxSize = float2 { maxWidth, maxHeight };
    }

    BoxConstraints::BoxConstraints(const float2 minSize, const float2 maxSize)
    {
        _minSize = minSize;
        _maxSize = maxSize;
    }

    BoxConstraints::BoxConstraints(const resolve_command& command)
    {
        _minSize = command.get_min_size();
        _maxSize = command.get_max_size();
    }

    BoxConstraints BoxConstraints::expand(const float width, const float height)
    {
        return BoxConstraints(width, width, height, height);
    }

    BoxConstraints BoxConstraints::loose(const Size size)
    {
        return BoxConstraints(0.f, size.width(), 0.f, size.height());
    }

    BoxConstraints BoxConstraints::tight(const Size size)
    {
        return BoxConstraints(size.width(), size.width(), size.height(), size.height());
    }

    // BoxConstraints BoxConstraints::tightForWidth(const float width)
    // {
    // }

    // BoxConstraints BoxConstraints::tightForHeight(const float height)
    // {
    // }

    // BoxConstraints BoxConstraints::tightForFiniteWidth(const float width)
    // {
    // }

    // BoxConstraints BoxConstraints::tightForFiniteHeight(const float height)
    // {
    // }

    Size BoxConstraints::biggest() const
    {
        return Size(constrainWidth(), constrainHeight());
    }

    // BoxConstraints BoxConstraints::flipped() const
    // {
    // }

    bool BoxConstraints::hasBoundedHeight() const
    {
        return (_maxSize.y != infinity<float>);
    }

    bool BoxConstraints::hasBoundedWidth() const
    {
        return (_maxSize.x != infinity<float>);
    }

    // bool BoxConstraints::hasInfiniteHeight() const
    // {
    // }

    // bool BoxConstraints::hasInfiniteWidth() const
    // {
    // }

    // bool BoxConstraints::hasTightHeight() const
    // {
    // }

    // bool BoxConstraints::hasTightWidth() const
    // {
    // }

    // bool BoxConstraints::isNormalized() const
    // {
    // }

    bool BoxConstraints::isTight() const
    {
        return (_minSize == _maxSize);
    }

    float BoxConstraints::maxHeight() const
    {
        return _maxSize.y;
    }

    float BoxConstraints::maxWidth() const
    {
        return _maxSize.x;
    }

    Size BoxConstraints::smallest() const
    {
        return Size(constrainWidth(0.f), constrainHeight(0.f));
    }

    Size BoxConstraints::constrain(const Size size) const
    {
        return Size(constrainWidth(size.width()), constrainHeight(size.height()));
    }

    Size BoxConstraints::constrainDimensions(const float width, const float height) const
    {
        return Size(constrainWidth(width), constrainHeight(height));
    }

    float BoxConstraints::constrainHeight(const float height) const
    {
        return glm::clamp(height, _minSize.y, _maxSize.y);
    }

    Size BoxConstraints::constrainSizeAndAttemptToPreserveAspectRatio(const Size size) const
    {
        if (isTight()) {
            return smallest();
        }
        float _width = size.width();
        float _height = size.height();
        if (_width <= 0.f) {
            log_error("BoxConstraints", "Impossible to constrain size and attempt to preserve aspect ratio because of illegal width <= 0.f.");
        }
        if (_height <= 0.f) {
            log_error("BoxConstraints", "Impossible to constrain size and attempt to preserve aspect ratio because of illegal height <= 0.f.");
        }
        float _aspectRatio = _width / _height;
        if (_width > _maxSize.x) {
            _width = _maxSize.x;
            _height = _width / _aspectRatio;
        }
        if (_height > _maxSize.y) {
            _height = _maxSize.y;
            _width = _height * _aspectRatio;
        }
        if (_width < _minSize.x) {
            _width = _minSize.x;
            _height = _width / _aspectRatio;
        }
        if (_height < _minSize.y) {
            _height = _minSize.y;
            _width = _height * _aspectRatio;
        }
        return Size(constrainWidth(_width), constrainHeight(_height));
    }

    float BoxConstraints::constrainWidth(const float width) const
    {
        return glm::clamp(width, _minSize.x, _maxSize.x);
    }

}
}