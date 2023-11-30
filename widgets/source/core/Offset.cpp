#include <bungeegum_widgets/core/Offset.hpp>

namespace bungeegum {
namespace widgets {

    Offset::Offset(const float dx, const float dy)
        : _value { dx, dy }
    {
    }

    Offset Offset::fromDirection(const float direction, const float distance)
    {
        return Offset(distance * std::cosf(direction), distance * std::sinf(direction));
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

    Offset Offset::scale(const float scaleX, const float scaleY)
    {
        _value *= float2 { scaleX, scaleY };
        return *this;
    }

    Offset Offset::translate(const float translateX, const float translateY)
    {
        _value += float2 { translateX, translateY };
        return *this;
    }

    Offset Offset::operatorUnaryNegation()
    {
        _value = float2 { 1.f, 1.f } - _value;
        return *this;
    }

    // Offset& Offset::operatorIntegerDivision(const float operand)
    // {
    // }

    Offset Offset::operator%(const float operand)
    {
        _value = glm::mod(_value, operand);
        return *this;
    }

    Offset Offset::operator*(const float operand)
    {
        _value *= operand;
        return *this;
    }

    Offset Offset::operator+(const float operand)
    {
        _value += operand;
        return *this;
    }

    Offset Offset::operator+(const Offset& other)
    {
        _value += other._value;
        return *this;
    }

    Offset Offset::operator-(const float operand)
    {
        _value -= operand;
        return *this;
    }

    Offset Offset::operator/(const float operand)
    {
        _value /= operand;
        return *this;
    }

    bool Offset::operator<(const Offset& other)
    {
        return ((_value.x < other.dx())
            && (_value.y < other.dy()));
    }

    bool Offset::operator<=(const Offset& other)
    {
        return ((_value.x <= other.dx())
            && (_value.y <= other.dy()));
    }

    bool Offset::operator==(const Offset& other)
    {
        return ((_value.x == other.dx())
            && (_value.y == other.dy()));
    }

    bool Offset::operator>(const Offset& other)
    {
        return ((_value.x > other.dx())
            && (_value.y > other.dy()));
    }

    bool Offset::operator>=(const Offset& other)
    {
        return ((_value.x >= other.dx())
            && (_value.y >= other.dy()));
    }

    Offset::operator float2() const
    {
        return _value;
    }
}
}