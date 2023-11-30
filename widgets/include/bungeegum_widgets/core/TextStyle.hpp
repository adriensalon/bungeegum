#pragma once

#include <bungeegum/asset/font.hpp>
#include <bungeegum_widget/core/Paint.hpp>
#include <bungeegum_widget/core/Color.hpp>
#include <bungeegum_widget/core/TextLeadingDistribution.hpp>

namespace bungeegum {
namespace widgets {

    struct TextStyle {

		TextStyle& background(const std::optional<Paint>& value);

		TextStyle& backgroundColor(const std::optional<Color> value);
		

		/// @brief The paint drawn as a foreground for the text.
		TextStyle& foreground(const std::optional<Paint>& value);

		/// @brief The height of this text span, as a multiple of the font size.
		TextStyle& height(const double value);

		/// @brief How the vertical space added by the height multiplier should be distributed over
		/// and under the text.
		TextStyle& leadingDistribution(const std::optional<TextLeadingDistribution> value);

		/// @brief The amount of space (in logical pixels) to add between each letter. A negative 
		/// value can be used to bring the letters closer.
		TextStyle& letterSpacing(const std::optional<double> value);

		// locale


		TextStyle& setFont(const font& value);

		TextStyle& 

    };

}
}