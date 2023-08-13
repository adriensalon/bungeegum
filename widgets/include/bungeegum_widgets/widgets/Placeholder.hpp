#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that draws a box that represents where other widgets will one day be added.
	/// @details This widget is useful during development to indicate that the interface is not yet
	/// complete. By default, the placeholder is sized to fit its container. If the placeholder is 
	/// in an unbounded space, it will size itself according to the given fallbackWidth and 
	/// fallbackHeight.
    struct Placeholder {

        /// @brief The child contained by the placeholder box.
        /// @details Defaults to std::nullopt
        Placeholder& child(const std::optional<runtime_widget>& value);

		/// @brief The color to draw the placeholder box. 
		Placeholder& color(const Color& value);

		/// @brief The height to use when the placeholder is in a situation with an unbounded
		/// width.
		Placeholder& fallbackHeight(const double value);

		/// @brief The width to use when the placeholder is in a situation with an unbounded width. 
		Placeholder& fallbackWidth(const double value);

		/// @brief The width of the lines in the placeholder box. 
		Placeholder& strokeWidth(const double value);


    private:
		std::optional<runtime_widget> _child = std::nullopt;
	
    };

}
}