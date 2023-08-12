#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief An immutable 2D floating-point offset.
    /// @details Imitates https://api.flutter.dev/flutter/dart-ui/Offset-class.html
    /// Generally speaking, Offsets can be interpreted in two ways:
    /// 	- 	As representing a point in Cartesian space a specified distance from a separately-
    ///			maintained origin.
    /// 	- 	As a vector that can be applied to coordinates.
    /// Because a particular Offset can be interpreted as one sense at one time then as the other
    /// sense at a later time, the same class is used for both senses.
    struct Offset {

        /// @brief Creates an offset. The first argument sets dx, the horizontal component, and the
        /// second sets dy, the vertical component.
        Offset(const float1 dx, const float1 dy);

        /// @brief Creates an offset from its direction and distance.
        /// @details The direction is in radians clockwise from the positive x-axis. The distance
        /// can be omitted, to create a unit vector (distance = 1.0).
        static Offset fromDirection(const float1 direction, const float1 distance = 1.f);

        /// @brief The angle of this offset as radians clockwise from the positive x-axis, in the
        /// range -pi to pi, assuming positive values of the x-axis go to the right and positive
        /// values of the y-axis go down.
        /// @details Zero means that dy is zero and dx is zero or positive. Values from zero to
        /// pi/2 indicate positive values of dx and dy, the bottom-right quadrant. Values from pi/2
        /// to pi indicate negative values of dx and positive values of dy, the bottom-left
        /// quadrant. Values from zero to -pi/2 indicate positive values of dx and negative values
        /// of dy, the top-right quadrant. Values from -pi/2 to -pi indicate negative values of dx
        /// and dy, the top-left quadrant. When dy is zero and dx is negative, the direction is pi.
        /// When dx is zero, direction is pi/2 if dy is positive and -pi/2 if dy is negative.
        float1 direction() const;

        /// @brief The magnitude of the offset.
        /// @details If you need this value to compare it to another Offset's distance, consider
        /// using distanceSquared instead, since it is cheaper to compute.
        float1 distance() const;

        /// @brief The square of the magnitude of the offset.
        /// @details This is cheaper than computing the distance itself.
        float1 distanceSquared() const;

        /// @brief The x component of the offset.
        /// @details The y component is given by dy.
        float1 dx() const;

        /// @brief The y component of the offset.
        /// @details The x component is given by dx.
        float1 dy() const;

        /// @brief Whether both components are finite.
        bool isFinite() const;

        /// @brief Returns true if either component is infinity, and false if both are finite.
        bool isInfinite() const;

        /// @brief Returns a new offset with the x component scaled by scaleX and the y component
        /// scaled by scaleY.
        /// @details If the two scale arguments are the same, consider using the * operator
        /// instead. If the two arguments are -1, consider using the operatorUnaryNegation instead.
        Offset scale(const float1 scaleX, const float1 scaleY);

        /// @brief Returns a new offset with translateX added to the x component and translateY
        /// added to the y component.
        /// @details If the arguments come from another Offset, consider using the + or - operators
        /// instead.
        Offset translate(const float1 translateX, const float1 translateY);

        // Operators
        Offset operatorUnaryNegation();
        // Offset& operatorIntegerDivision(const float1 operand);
        Offset operator%(const float1 operand);
        Offset operator*(const float1 operand);
        Offset operator+(const Offset& other);
        Offset operator+(const float1 operand);
        Offset operator-(const float1 operand);
        Offset operator/(const float1 operand);
        bool operator<(const Offset& other);
        bool operator<=(const Offset& other);
        bool operator==(const Offset& other);
        bool operator>(const Offset& other);
        bool operator>=(const Offset& other);

        operator float2() const;

        Offset() = default;
        Offset(const Offset& other) = default;
        Offset& operator=(const Offset& other) = default;
        Offset(Offset&& other) = default;
        Offset& operator=(Offset&& other) = default;

    private:
        float2 _value = zero<float2>;
    };
}

template <>
inline widgets::Offset zero<widgets::Offset> = { zero<float1>, zero<float1> };

template <>
inline widgets::Offset infinity<widgets::Offset> = { infinity<float1>, infinity<float1> };
}