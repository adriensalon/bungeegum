// import from ttf with imgui to imfont direct

#pragma once

#include <filesystem>
#include <memory>
#include <string>

namespace bungeegum {
namespace detail {

    /// @brief
    struct font {
        font() = default;
        font(const font& other);
        font& operator=(const font& other);
        font(font&& other);
        font& operator=(font&& other);

        static font import(const std::string& hlsl_fragment);

    private:
        struct font_data;
        std::shared_ptr<font_data> _data = nullptr;
    };

    struct glyph { // create from font and codepoint
    };

}
}