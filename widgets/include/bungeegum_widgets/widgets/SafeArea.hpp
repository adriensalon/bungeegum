#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/EdgeInsets.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that insets its child by sufficient padding to avoid intrusions by the 
	/// operating system.
	/// @details For example, this will indent the child by enough to avoid the status bar at the
	/// top of the screen. It will also indent the child by the amount necessary to avoid The Notch
	/// on the iPhone X, or other similar creative physical features of the display. When a minimum
	/// padding is specified, the greater of the minimum padding or the safe area padding will be
	/// applied.
    struct SafeArea {

		/// @brief Whether to avoid system intrusions on the bottom side of the screen.
		SafeArea& bottom(const bool value);

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        SafeArea& child(const runtime_widget& value);

		/// @brief Whether to avoid system intrusions on the left. 
		SafeArea& left(const bool value);

		/// @brief Specifies whether the SafeArea should maintain the bottom 
		/// MediaQueryData.viewPadding instead of the bottom MediaQueryData.padding, defaults to 
		/// false.
		SafeArea& maintainBottomViewPadding(const bool value);
		
		/// @brief This minimum padding to apply.
		SafeArea& minimum(const EdgeInsets& value);
		
		/// @brief Whether to avoid system intrusions on the right.
		SafeArea& right(const bool value);

		/// @brief Whether to avoid system intrusions on the top.
		SafeArea& top(const bool value);

    private:
		EdgeInsets _minimum = {};
		bool _bottom = true;
		bool _left = true;
		bool _right = true;
		bool _top = true;
    };

}
}