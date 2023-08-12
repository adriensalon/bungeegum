#include <bungeegum_widgets/core/EdgeInsets.hpp>

namespace bungeegum {
namespace widgets {

    EdgeInsets EdgeInsets::all(const float1 value)
    {
        return fromLTRB(value, value, value, value);
    }

    EdgeInsets EdgeInsets::fromLTRB(const float1 left, const float1 top, const float1 right, const float1 bottom)
    {
        EdgeInsets _edgeInsets;
        _edgeInsets._value = float4 { left, top, right, bottom };
        return _edgeInsets;
    }

    EdgeInsets EdgeInsets::symmetric(const float1 vertical, const float1 horizontal)
    {
        return fromLTRB(horizontal, vertical, horizontal, vertical);
    }

    float1 EdgeInsets::bottom() const
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

    float1 EdgeInsets::horizontal() const
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

    float1 EdgeInsets::left() const
    {
        return _value.x;
    }

    float1 EdgeInsets::right() const
    {
        return _value.z;
    }

    float1 EdgeInsets::top() const
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

    float1 EdgeInsets::vertical() const
    {
        return (top() + bottom());
    }

}
}