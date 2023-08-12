#pragma once

// https://api.flutter.dev/flutter/dart-ui/Size-class.html

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct Offset;

    struct Size {

        // Constructors
        Size(const float1 width, const float1 height);
        Size(const float2 size);

        // Factories
        static Size fromHeight(const float1 height);
        static Size fromRadius(const float1 radius);
        static Size fromWidth(const float1 width);
        static Size square(const float1 dimension);

        // Properties
        float1 aspectRatio() const;
        Size flipped() const;
        float1 height() const;
        bool isEmpty() const;
        bool isFinite() const;
        bool isInfinite() const;
        float1 longestSide() const;
        float1 shortestSide() const;
        float1 width() const;
        Offset bottomCenter(const Offset& origin) const;
        Offset bottomLeft(const Offset& origin) const;
        Offset bottomRight(const Offset& origin) const;
        Offset center(const Offset& origin) const;
        Offset centerLeft(const Offset& origin) const;
        Offset centerRight(const Offset& origin) const;
        bool contains(const Offset& offset) const;
        Offset topCenter(const Offset& origin) const;
        Offset topLeft(const Offset& origin) const;
        Offset topRight(const Offset& origin) const;

        // Operators
        // Size& operatorIntegerDivision(const float1 operand);
        Size& operator%(const float1 operand);
        Size& operator*(const float1 operand);
        Size& operator+(const Size& other);
        Size& operator+(const Offset& other);
        Size& operator-(const Offset& other);
        Size& operator-(const Size& other);
        Size& operator/(const float1 operand);
        bool operator<(const Offset& other);
        bool operator<=(const Offset& other);
        bool operator==(const Offset& other);
        bool operator>(const Offset& other);
        bool operator>=(const Offset& other);
        bool operator<(const Size& other);
        bool operator<=(const Size& other);
        bool operator==(const Size& other);
        bool operator>(const Size& other);
        bool operator>=(const Size& other);

        operator float2() const;

        Size() = default;
        Size(const Size& other) = default;
        Size& operator=(const Size& other) = default;
        Size(Size&& other) = default;
        Size& operator=(Size&& other) = default;

    private:
        float2 _value = { 0.f, 0.f };
    };
}

template <>
inline widgets::Size zero<widgets::Size> = { zero<float1>, zero<float1> };

template <>
inline widgets::Size infinity<widgets::Size> = { infinity<float1>, infinity<float1> };
}