#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Color.hpp>
#include <bungeegum_widgets/core/TextOverflow.hpp>

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

		/// @brief The text to display. 
		Text& data(const std::optional<std::string>& value);
		
		// locale

		/// @brief An optional maximum number of lines for the text to span, wrapping if necessary. If the text exceeds the given number of lines, it will be truncated according to overflow.
		Text& maxLines(const std::optional<unsigned int> value);

		/// @brief  
		Text& overflow(const std::optional<TextOverflow> value);

		/// @brief The color to use when painting the selection.
		Text& selectionColor(const std::optional<Color> value);

		// semantics label

		// soft wrap

		// strutstyle

		// text style

		// text align

		// text direction

		// text height behavior

		/// @brief The number of font pixels for each logical pixel.
		Text& textScaleFactor(const std::optional<double> value);

		// text span : no only richtext !

		// text width basis

    private:
    };
}

}