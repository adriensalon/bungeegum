// uniform inputs only
// load from file with std or set from string

#pragma once

#include <filesystem>
#include <memory>
#include <string>

namespace bungeegum {
namespace detail {

    /// @brief
    struct shader {
        shader() = default;
        shader(const shader& other);
        shader& operator=(const shader& other);
        shader(shader&& other);
        shader& operator=(shader&& other);

        static shader import(const std::string& hlsl_fragment);

    private:
        struct shader_data;
        std::shared_ptr<shader_data> _data = nullptr;
    };
}
}