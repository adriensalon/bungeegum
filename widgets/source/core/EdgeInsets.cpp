#include <bungeegum_widgets/core/EdgeInsets.hpp>

namespace bungeegum {
namespace widgets {

    EdgeInsets EdgeInsets::all(const float value)
    {
        return fromLTRB(value, value, value, value);
    }

    EdgeInsets EdgeInsets::fromLTRB(const float left, const float top, const float right, const float bottom)
    {
        EdgeInsets _edgeInsets;
        _edgeInsets._value = float4 { left, top, right, bottom };
        return _edgeInsets;
    }

    EdgeInsets EdgeInsets::symmetric(const float vertical, const float horizontal)
    {
        return fromLTRB(horizontal, vertical, horizontal, vertical);
    }

    float EdgeInsets::bottom() const
    {
        return _value.w;
    }

    Offset EdgeInsets::bottomLeft() const
    {
        return Offset(left(), -bottom());
    }

    Offset EdgeInsets::bottomRight() const
    {
        return Offset(-right(), -bottom());
    }

    Size EdgeInsets::collapsedSize() const
    {
        return Size(horizontal(), vertical());
    }

    EdgeInsets EdgeInsets::flipped() const
    {
        return fromLTRB(right(), bottom(), left(), top());
    }

    float EdgeInsets::horizontal() const
    {
        return (left() + right());
    }

    bool EdgeInsets::isNonNegative() const
    {
        return ((left() >= 0.f)
            && (right() >= 0.f)
            && (top() >= 0.f)
            && (bottom() >= 0.f));
    }

    float EdgeInsets::left() const
    {
        return _value.x;
    }

    float EdgeInsets::right() const
    {
        return _value.z;
    }

    float EdgeInsets::top() const
    {
        return _value.y;
    }

    Offset EdgeInsets::topLeft() const
    {
        return Offset(left(), top());
    }

    Offset EdgeInsets::topRight() const
    {
        return Offset(-right(), top());
    }

    float EdgeInsets::vertical() const
    {
        return (top() + bottom());
    }

}
}