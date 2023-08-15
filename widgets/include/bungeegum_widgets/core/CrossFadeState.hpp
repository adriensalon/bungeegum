#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief Specifies which of two children to show. See AnimatedCrossFade.
	/// @brief The child that is shown will fade in, while the other will fade out.
    enum struct CrossFadeState {

        /// @brief Show the first child (AnimatedCrossFade.firstChild) and hide the second
		/// (AnimatedCrossFade.secondChild).
        showFirst,

        /// @brief Show the second child (AnimatedCrossFade.secondChild) and hide the first
		/// (AnimatedCrossFade.firstChild).
        showSecond
    };

}
}