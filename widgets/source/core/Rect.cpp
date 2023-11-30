#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Rect.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    Rect::Rect(const Offset& offset, const Size& size)
    {
        float _left = offset.dx();
        float _top = offset.dy();
        _value = { _left, _top, _left + size.width(), _top + size.height() };
    }

    Rect Rect::fromCenter(const Offset& center, const float width, const float height)
    {
        float _half_width = width * 0.5f;
        float _half_height = height * 0.5f;
        return fromLTRB(
            center.dx() - _half_width,
            center.dy() - _half_height,
            center.dx() + _half_width,
            center.dy() + _half_height);
    }

    Rect Rect::fromCircle(const Offset& center, const float radius)
    {
        float _diameter = radius * 2.f;
        return fromCenter(center, _diameter, _diameter);
    }

    Rect Rect::fromLTRB(const float left, const float top, const float right, const float bottom)
    {
        Rect _rect;
        _rect._value = { left, top, right, bottom };
        return _rect;
    }

    Rect Rect::fromLTWH(const float left, const float top, const float width, const float height)
    {
        return fromLTRB(left, top, left + width, top + height);
    }

    Rect Rect::fromPoints(const Offset& a, const Offset& b)
    {
        float _adx = a.dx();
        float _ady = a.dy();
        float _bdx = b.dx();
        float _bdy = b.dy();
        return fromLTRB(
            glm::min(_adx, _bdx),
            glm::min(_ady, _bdy),
            glm::max(_adx, _bdx),
            glm::max(_ady, _bdy));
    }

    // float Rect::bottom() const
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

    // float Rect::height() const
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

    // float Rect::left() const
    // {
    // }

    // float Rect::longestSide() const
    // {
    // }

    // float Rect::right() const
    // {
    // }

    // float Rect::shortestSide() const
    // {
    // }

    // Size Rect::size() const
    // {
    // }

    // float Rect::top() const
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

    // float Rect::width() const
    // {
    // }

}
}