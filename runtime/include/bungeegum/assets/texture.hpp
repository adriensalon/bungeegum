// load from file with stb_image

#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <bungeegum/core/math.hpp>

namespace bungeegum {
namespace detail {

    /// @brief
    struct texture {
        texture() = default;
        texture(const texture& other);
        texture& operator=(const texture& other);
        texture(texture&& other);
        texture& operator=(texture&& other);

        texture(const std::vector<float>& float_pixels);

    private:
        struct texture_data;
        std::shared_ptr<texture_data> _data = nullptr;
    };
}
}