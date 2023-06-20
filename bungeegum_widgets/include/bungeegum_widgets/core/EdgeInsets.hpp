#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief An immutable set of offsets in each of the four cardinal directions.
    /// @details Typically used for an offset from each of the four sides of a box. For example,
    /// the padding inside a box can be represented using this class.
    struct EdgeInsets {

        /// @brief Creates insets where all the offsets are value.
        static EdgeInsets all(const float1 value);

        /// @brief Creates insets from offsets from the left, top, right, and bottom.
        static EdgeInsets fromLTRB(const float1 left, const float1 top, const float1 right, const float1 bottom);

        /// @brief Creates insets with symmetrical vertical and horizontal offsets.
        static EdgeInsets symmetric(const float1 vertical, const float1 horizontal);

        /// @brief The offset from the bottom.
        float1 bottom() const;

        /// @brief An Offset describing the vector from the bottom left of a rectangle to the
        /// bottom left of that rectangle inset by this object.
        Offset bottomLeft() const;

        /// @brief An Offset describing the vector from the bottom right of a rectangle to the
        /// bottom right of that rectangle inset by this object.
        Offset bottomRight() const;

        /// @brief The size that this EdgeInsets would occupy with an empty interior.
        Size collapsedSize() const;

        /// @brief An EdgeInsets with top and bottom as well as left and right flipped.
        EdgeInsets flipped() const;

        /// @brief The total offset in the horizontal direction.
        float1 horizontal() const;

        /// @brief Whether every dimension is non-negative.
        bool isNonNegative() const;

        /// @brief The offset from the left.
        float1 left() const;

        /// @brief The offset from the right.
        float1 right() const;

        /// @brief The offset from the top.
        float1 top() const;

        /// @brief An Offset describing the vector from the top left of a rectangle to the top
        /// left of that rectangle inset by this object.
        Offset topLeft() const;

        /// @brief An Offset describing the vector from the top right of a rectangle to the top
        /// right of that rectangle inset by this object.
        Offset topRight() const;

        /// @brief The total offset in the vertical direction.
        float1 vertical() const;

        //
        //
        //
        // methods
        // operators

    private:
        float4 _value = zero<float4>; // LTRB
    };

}
}