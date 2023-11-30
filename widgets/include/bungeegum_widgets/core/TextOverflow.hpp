#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How overflowing text should be handled.
	/// @details A TextOverflow can be passed to Text and RichText via their Text.overflow and 
	/// RichText.overflow properties respectively.
    enum struct TextOverflow {

        /// @brief Clip the overflowing text to fix its container.
        clip,

        /// @brief Fade the overflowing text to transparent.
        fade,

        /// @brief Use an ellipsis to indicate that the text has overflowed.
        ellipsis,

        /// @brief Render overflowing text outside of its container.
        visible
    };

}
}