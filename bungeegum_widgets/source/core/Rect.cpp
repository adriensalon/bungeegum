#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Rect.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    Rect::Rect(const Offset& offset, const Size& size)
    {
        float1 _left = offset.dx();
        float1 _top = offset.dy();
        _value = { _left, _top, _left + size.width(), _top + size.height() };
    }

    Rect Rect::fromCenter(const Offset& center, const float1 width, const float1 height)
    {
        float1 _half_width = width * 0.5f;
        float1 _half_height = height * 0.5f;
        return fromLTRB(
            center.dx() - _half_width,
            center.dy() - _half_height,
            center.dx() + _half_width,
            center.dy() + _half_height);
    }

    Rect Rect::fromCircle(const Offset& center, const float1 radius)
    {
        float1 _diameter = radius * 2.f;
        return fromCenter(center, _diameter, _diameter);
    }

    Rect Rect::fromLTRB(const float1 left, const float1 top, const float1 right, const float1 bottom)
    {
        Rect _rect;
        _rect._value = { left, top, right, bottom };
        return _rect;
    }

    Rect Rect::fromLTWH(const float1 left, const float1 top, const float1 width, const float1 height)
    {
        return fromLTRB(left, top, left + width, top + height);
    }

    Rect Rect::fromPoints(const Offset& a, const Offset& b)
    {
        float1 _adx = a.dx();
        float1 _ady = a.dy();
        float1 _bdx = b.dx();
        float1 _bdy = b.dy();
        return fromLTRB(
            glm::min(_adx, _bdx),
            glm::min(_ady, _bdy),
            glm::max(_adx, _bdx),
            glm::max(_ady, _bdy));
    }

    // float1 Rect::bottom() const
    // {
    // }

    // Offset Rect::bottomCenter() const
    // {
    // }

    // Offset Rect::bottomLeft() const
    // {
    // }

    // Offset Rect::bottomRight() const
    // {
    // }

    // Offset Rect::center() const
    // {
    // }

    // Offset Rect::centerLeft() const
    // {
    // }

    // Offset Rect::centerRight() const
    // {
    // }

    // float1 Rect::height() const
    // {
    // }

    // bool Rect::isEmpty() const
    // {
    // }

    // bool Rect::isFinite() const
    // {
    // }

    // bool Rect::isInfinite() const
    // {
    // }

    // float1 Rect::left() const
    // {
    // }

    // float1 Rect::longestSide() const
    // {
    // }

    // float1 Rect::right() const
    // {
    // }

    // float1 Rect::shortestSide() const
    // {
    // }

    // Size Rect::size() const
    // {
    // }

    // float1 Rect::top() const
    // {
    // }

    // Offset Rect::topCenter() const
    // {
    // }

    // Offset Rect::topLeft() const
    // {
    // }

    // Offset Rect::topRight() const
    // {
    // }

    // float1 Rect::width() const
    // {
    // }

}
}