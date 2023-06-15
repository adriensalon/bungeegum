#pragma once

// https://api.flutter.dev/flutter/dart-ui/Offset-class.html

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct Offset {
        Offset() = default;
        Offset(const Offset& other) = default;
        Offset& operator=(const Offset& other) = default;
        Offset(Offset&& other) = default;
        Offset& operator=(Offset&& other) = default;

        Offset(const float dx, const float dy);

        float direction() const;
        float distance() const;
        float distanceSquared() const;
        float dx() const;
        float dy() const;
        bool isFinite() const;
        bool isInfinite() const;

        Offset& scale(const float scaleX, const float scaleY);
        Offset& translate(const float translateX, const float translateY);

        static Offset fromDirection(const float direction, const float distance = 1.f);

    private:
        float2 _value = { 0.f, 0.f };
    };

}
}