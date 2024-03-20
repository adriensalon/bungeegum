#pragma once

#include <bungeegum_widgets/core/TextOverflow.hpp>
#include <bungeegum_widgets/core/TextStyle.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A run of text with a single style.
    /// @details The Text widget displays a string of text with single style. The string might
    /// break across multiple lines or might all be displayed on the same line depending on the
    /// layout constraints. The style argument is optional. When omitted, the text will use the
    /// style from the closest enclosing DefaultTextStyle. If the given style's TextStyle.inherit
    /// property is true (the default), the given style will be merged with the closest enclosing
    /// DefaultTextStyle. This merging behavior is useful, for example, to make the text bold
    /// while using the default font family and size.
    struct Text {

        Text& data(const std::optional<std::string>& value);

        // locale

        Text& maxLines(const std::optional<std::size_t> value);

        /// @brief How visual overflow should be handled.
        /// @details If this is null TextStyle.overflow will be used, otherwise the value from the 
        /// nearest DefaultTextStyle ancestor will be used.
        Text& overflow(const std::optional<TextOverflow> value);


        Text& style(const TextStyle& value);

    private:
        TextOverflow
    };
}

}