#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct Offset;

    struct Size;

    /// @brief An immutable, 2D, axis-aligned, floating-point rectangle whose coordinates
    /// are relative to a given origin.
    /// @details Imitates https://api.flutter.dev/flutter/dart-ui/Rect-class.html
    struct Rect {

        /// @brief A Rect can be created from an Offset and a Size.
        Rect(const Offset& offset, const Size& size);

        /// @brief Constructs a rectangle from its center point, width, and height.
        /// @details The center argument is assumed to be an offset from the origin.
        static Rect fromCenter(const Offset& center, const float1 width, const float1 height);

        /// @brief Construct a rectangle that bounds the given circle.
        /// @details The center argument is assumed to be an offset from the origin.
        static Rect fromCircle(const Offset& center, const float1 radius);

        /// @brief Construct a rectangle from its left, top, right, and bottom edges.
        static Rect fromLTRB(const float1 left, const float1 top, const float1 right, const float1 bottom);

        /// @brief Construct a rectangle from its left and top edges, its width, and its height.
        /// @details To construct a Rect from an Offset and a Size, you can use the constructor.
        static Rect fromLTWH(const float1 left, const float1 top, const float1 width, const float1 height);

        /// @brief Construct the smallest rectangle that encloses the given offsets, treating
        /// them as vectors from the origin.
        static Rect fromPoints(const Offset& a, const Offset& b);

        /// @brief The offset of the bottom edge of this rectangle from the y axis.
        float1 bottom() const;

        /// @brief The offset to the center of the bottom edge of this rectangle.
        Offset bottomCenter() const;

        /// @brief The offset to the intersection of the bottom and left edges of this rectangle.
        Offset bottomLeft() const;

        /// @brief The offset to the intersection of the bottom and right edges of this rectangle.
        Offset bottomRight() const;

        /// @brief The offset to the point halfway between the left and right and the top and
        /// bottom edges of this rectangle.
        Offset center() const;

        /// @brief The offset to the center of the left edge of this rectangle.
        Offset centerLeft() const;

        /// @brief The offset to the center of the left edge of this rectangle.
        Offset centerRight() const;

        /// @brief The distance between the top and bottom edges of this rectangle.
        float1 height() const;

        /// @brief Whether this rectangle encloses a non-zero area. Negative areas are considered
        /// empty.
        bool isEmpty() const;

        /// @brief Whether all coordinates of this rectangle are finite.
        bool isFinite() const;

        /// @brief Whether any of the coordinates of this rectangle are equal to positive infinity.
        bool isInfinite() const;

        /// @brief The offset of the left edge of this rectangle from the x axis.
        float1 left() const;

        /// @brief The greater of the magnitudes of the width and the height of this rectangle.
        float1 longestSide() const;

        /// @brief The offset of the right edge of this rectangle from the x axis.
        float1 right() const;

        /// @brief The lesser of the magnitudes of the width and the height of this rectangle.
        float1 shortestSide() const;

        /// @brief The distance between the upper-left corner and the lower-right corner of this
        /// rectangle.
        Size size() const;

        /// @brief The offset of the top edge of this rectangle from the y axis.
        float1 top() const;

        /// @brief The offset to the center of the top edge of this rectangle.
        Offset topCenter() const;

        /// @brief The offset to the intersection of the top and left edges of this rectangle.
        Offset topLeft() const;

        /// @brief The offset to the intersection of the top and right edges of this rectangle.
        Offset topRight() const;

        /// @brief The distance between the left and right edges of this rectangle.
        float1 width() const;

        /// @brief Whether the point specified by the given offset (which is assumed to be relative
        /// to the origin) lies between the left and right and the top and bottom edges of this
        /// rectangle.
        /// @details Rectangles include their top and left edges but exclude their bottom and right
        /// edges.
        bool contains(const Offset offset) const;

        /// @brief Returns a new rectangle with edges moved inwards by the given delta.
        /// @param delta
        Rect deflate(const float delta) const;

        Rect() = default;
        Rect(const Rect& other) = default;
        Rect& operator=(const Rect& other) = default;
        Rect(Rect&& other) = default;
        Rect& operator=(Rect&& other) = default;

    private:
        float4 _value = zero<float4>; // LTRB
    };
}

template <>
inline widgets::Rect zero<widgets::Rect> = widgets::Rect::fromLTRB(zero<float1>, zero<float1>, zero<float1>, zero<float1>);

// largest ?
}