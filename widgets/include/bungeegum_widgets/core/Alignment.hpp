#pragma once

// https://api.flutter.dev/flutter/painting/Alignment-class.html

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct Rect;

    struct Size;

    struct Offset;

    /// @brief A point within a rectangle.
    struct Alignment {

        // Constructors
        inline Alignment(const float1 x, const float1 y)
            : _value { x, y } {};

        // Factories
        static Alignment bottomCenter();
        static Alignment bottomLeft();
        static Alignment bottomRight();
        static Alignment center();
        static Alignment centerLeft();
        static Alignment centerRight();
        static Alignment topCenter();
        static Alignment topLeft();
        static Alignment topRight();

        // Properties
        float1 x() const;
        float1 y() const;

        // Methods
        Offset alongOffset(const Offset other);
        Offset alongSize(const Size other);
        Rect inscribe(const Size size, const Rect rect);
        Offset withinRect(const Rect rect);

        // Operators
        // Size& operatorIntegerDivision(const float1 operand);
        // Size&
        // operator%(const float1 operand);
        // Size& operator*(const float1 operand);
        // Size& operator+(const Offset& other);
        // Size& operator-(const Offset& other);
        // Size& operator-(const Size& other);
        // Size& operator/(const float1 operand);
        // bool operator<(const Offset& other);
        // bool operator<=(const Offset& other);
        // bool operator==(const Offset& other);
        // bool operator>(const Offset& other);
        // bool operator>=(const Offset& other);
        // bool operator<(const Size& other);
        // bool operator<=(const Size& other);
        // bool operator==(const Size& other);
        // bool operator>(const Size& other);
        // bool operator>=(const Size& other);

        operator float2() const;

        Alignment() = default;
        Alignment(const Alignment& other) = default;
        Alignment& operator=(const Alignment& other) = default;
        Alignment(Alignment&& other) = default;
        Alignment& operator=(Alignment&& other) = default;

    private:
        float2 _value = zero<float2>;
    };

}
}