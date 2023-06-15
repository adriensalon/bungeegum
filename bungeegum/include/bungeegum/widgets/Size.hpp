#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct Size {
        Size() = default;
        Size(const Size& other) = default;
        Size& operator=(const Size& other) = default;
        Size(Size&& other) = default;
        Size& operator=(Size&& other) = default;
        Size(const float width, const float height);

    private:
        float2 _value = { 0.f, 0.f };
    };

}
}