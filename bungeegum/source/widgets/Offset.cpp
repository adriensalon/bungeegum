#include <bungeegum/widgets/align.hpp>

namespace bungeegum {
namespace widgets {

    Offset::Offset(const float dx, const float dy)
        : _value { dx, dy }
    {
    }

    float Offset::direction() const
    {
        return std::atan2f(_value.y, _value.x);
    }

    float Offset::distance() const
    {
        return std::sqrtf(distanceSquared());
    }

    float Offset::distanceSquared() const
    {
        float2 _squared = _value * _value;
        return _squared.x + _squared.y;
    }

    float Offset::dx() const
    {
        return _value.x;
    }

    float Offset::dy() const
    {
        return _value.y;
    }

    bool Offset::isFinite() const
    {
        return is_finite<float2>(_value);
    }

    bool Offset::isInfinite() const
    {
        return is_infinite<float2>(_value);
    }

    Offset& Offset::scale(const float scaleX, const float scaleY)
    {
        _value *= float2 { scaleX, scaleY };
        return *this;
    }

    Offset& Offset::translate(const float translateX, const float translateY)
    {
        _value += float2 { translateX, translateY };
        return *this;
    }

    Offset Offset::fromDirection(const float direction, const float distance)
    {
        return Offset(distance * std::cosf(direction), distance * std::sinf(direction));
    }

}
}