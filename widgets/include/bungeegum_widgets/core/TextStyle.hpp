#pragma once

#include <bungeegum_widgets/core/Paint.hpp>
#include <bungeegum_widgets/core/TextDecoration.hpp>
#include <bungeegum_widgets/core/TextDecorationStyle.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief An immutable style describing how to format and paint text.
    struct TextStyle {

        TextStyle& background(const std::optional<Paint>& value);

        TextStyle& color(const std::optional<Color>& value);

        TextStyle decoration(const std::optional<TextDecoration>& value);
        
        TextStyle& decorationColor(const std::optional<Color>& value);

        TextStyle decorationStyle(const std::optional<TextDecorationStyle>& value);

        TextStyle decorationThickness(const std::optional<float> value);
    };

}
}