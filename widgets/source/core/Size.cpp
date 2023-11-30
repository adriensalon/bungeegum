#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    Size::Size(const float width, const float height)
        : _value { width, height }
    {
    }

    Size::Size(const float2 size)
        : _value { size }
    {
    }

    Size Size::fromHeight(const float height)
    {
        return Size(infinity<float>, height);
    }

    Size Size::fromRadius(const float radius)
    {
        return Size::square(radius * 2.f);
    }

    Size Size::fromWidth(const float width)
    {
        return Size(width, infinity<float>);
    }

    Size Size::square(const float dimension)
    {
        return Size(dimension, dimension);
    }

    float Size::aspectRatio() const
    {
        if (_value.y != 0.f)
            return _value.x / _value.y;
        if (_value.x > 0.f)
            return infinity<float>;
        if (_value.x < 0.f)
            return -infinity<float>; // negative infinity ?
        return 0.f;
    }

    Size Size::flipped() const
    {
        return Size(_value.y, _value.x);
    }

    float Size::height() const
    {
        return _value.y;
    }

    bool Size::isEmpty() const
    {
        return ((_value.x <= 0.f)
            || (_value.y <= 0.f));
    }

    bool Size::isFinite() const
    {
        return is_finite<float2>(_value);
    }

    bool Size::isInfinite() const
    {
        return is_infinite<float2>(_value);
    }

    float Size::longestSide() const
    {
        return glm::max(glm::abs(_value.x), glm::abs(_value.y));
    }

    float Size::shortestSide() const
    {
        return glm::min(glm::abs(_value.x), glm::abs(_value.y));
    }

    float Size::width() const
    {
        return _value.x;
    }

    Offset Size::bottomCenter(const Offset& origin) const
    {
        return Offset(origin.dx() + _value.x * 0.5f, origin.dy() + _value.y);
    }

    Offset Size::bottomLeft(const Offset& origin) const
    {
        return Offset(origin.dx(), origin.dy() + _value.y);
    }

    Offset Size::bottomRight(const Offset& origin) const
    {
        return Offset(origin.dx() + _value.x, origin.dy() + _value.y);
    }

    Offset Size::center(const Offset& origin) const
    {
        // possible to go 1x vec2 add 1x vec2 mul
        return Offset(origin.dx() + _value.x * 0.5f, origin.dy() + _value.y * 0.5f);
    }

    Offset Size::centerLeft(const Offset& origin) const
    {
        return Offset(origin.dx(), origin.dy() + _value.y * 0.5f);
    }

    Offset Size::centerRight(const Offset& origin) const
    {
        return Offset(origin.dx() + _value.x, origin.dy() + _value.y * 0.5f);
    }

    bool Size::contains(const Offset& offset) const
    {
        return ((offset.dx() >= 0.f)
            && (offset.dx() < _value.x)
            && (offset.dy() >= 0.f)
            && (offset.dy() < _value.y));
    }

    Offset Size::topCenter(const Offset& origin) const
    {
        return Offset(origin.dx() + _value.x * 0.5f, origin.dy());
    }

    Offset Size::topLeft(const Offset& origin) const
    {
        return origin;
    }

    Offset Size::topRight(const Offset& origin) const
    {
        return Offset(origin.dx() + _value.x, origin.dy());
    }

    // Size& Size::operatorIntegerDivision(const float operand)
    // {
    // }

    Size& Size::operator%(const float operand)
    {
        _value = glm::mod(_value, operand);
        return *this;
    }

    Size& Size::operator*(const float operand)
    {
        _value *= operand;
        return *this;
    }

    Size& Size::operator+(const Size& other)
    {
        _value += float2 { other.width(), other.height() };
        return *this;
    }

    Size& Size::operator+(const Offset& other)
    {
        _value += float2 { other.dx(), other.dy() };
        return *this;
    }

    Size& Size::operator-(const Offset& other)
    {
        _value -= float2 { other.dx(), other.dy() };
        return *this;
    }

    Size& Size::operator-(const Size& other)
    {
        _value -= float2 { other.width(), other.height() };
        return *this;
    }

    Size& Size::operator/(const float operand)
    {
        _value /= operand;
        return *this;
    }

    bool Size::operator<(const Offset& other)
    {
        return ((_value.x < other.dx())
            && (_value.y < other.dy()));
    }

    bool Size::operator<=(const Offset& other)
    {
        return ((_value.x <= other.dx())
            && (_value.y <= other.dy()));
    }

    bool Size::operator==(const Offset& other)
    {
        return ((_value.x == other.dx())
            && (_value.y == other.dy()));
    }

    bool Size::operator>(const Offset& other)
    {
        return ((_value.x > other.dx())
            && (_value.y > other.dy()));
    }

    bool Size::operator>=(const Offset& other)
    {
        return ((_value.x >= other.dx())
            && (_value.y >= other.dy()));
    }

    bool Size::operator<(const Size& other)
    {
        return ((_value.x < other.width())
            && (_value.y < other.height()));
    }

    bool Size::operator<=(const Size& other)
    {
        return ((_value.x <= other.width())
            && (_value.y <= other.height()));
    }

    bool Size::operator==(const Size& other)
    {
        return ((_value.x == other.width())
            && (_value.y == other.height()));
    }

    bool Size::operator>(const Size& other)
    {
        return ((_value.x > other.width())
            && (_value.y > other.height()));
    }

    bool Size::operator>=(const Size& other)
    {
        return ((_value.x >= other.width())
            && (_value.y >= other.height()));
    }

    Size::operator float2() const
    {
        return _value;
    }
}
}