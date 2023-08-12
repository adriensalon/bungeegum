#pragma once

#include <bungeegum/asset/texture.hpp>
#include <bungeegum/glue/simd.hpp>
#include <bungeegum_widgets/core/BlendMode.hpp>

namespace bungeegum {
namespace widgets {

    struct Image {
        Image() = default;
        Image(const Image& other) = default;
        Image& operator=(const Image& other) = default;
        Image(Image&& other) = default;
        Image& operator=(Image&& other) = default;

        uint1 height() const;

        uint1 width() const;

    private:
        texture _data = {};
    };
}

}