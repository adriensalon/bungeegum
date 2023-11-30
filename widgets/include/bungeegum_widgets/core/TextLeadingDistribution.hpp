#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How the "leading" is distributed over and under the text.
	/// @details Does not affect layout when TextStyle.height is not specified. The leading can 
	/// become negative, for example, when TextLeadingDistribution.even is used with a 
	/// TextStyle.height much smaller than 1.0.
    enum struct TextLeadingDistribution {

        /// @brief Distributes the leading of the text proportionally above and below the text,
		/// to the font's ascent/descent ratio.
		/// @details The leading of a text run is defined as TextStyle.height * TextStyle.fontSize 
		/// - TextStyle.fontSize. When TextStyle.height is not set, the text run uses the leading 
		/// specified by the font instead.
        proportional,

        /// @brief Distributes the "leading" of the text evenly above and below the text (i.e.
		/// evenly above the font's ascender and below the descender).
		/// @details The leading of a text run is defined as TextStyle.height * TextStyle.fontSize 
		/// - TextStyle.fontSize. When TextStyle.height is not set, the text run uses the leading
		/// specified by the font instead. The leading can become negative when TextStyle.height is
		/// smaller than 1.0. This is the default strategy used by CSS, known as "half-leading".
        even
    };

}
}