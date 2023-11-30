#pragma once

#include <bungeegum/core/runtime.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that makes its child partially transparent.
	/// @details This class paints its child into an intermediate buffer and then blends the child
	/// back into the scene partially transparent.
    struct Opacity {

		/// @brief The widget below this widget in the tree.
		/// @details This widget can only have one child. To lay out multiple children, let this
		/// widget's child be a widget such as Row, Column, or Stack, which have a children
		/// property, and then provide the children to that widget.
		Opacity& child(const std::optional<runtime_widget>& value);

		/// @brief The fraction to scale the child's alpha value.
		/// @details An opacity of 1.0 is fully opaque. An opacity of 0.0 is fully transparent
		/// (i.e., invisible).
		Opacity& opacity(const double value);

    private:
        friend struct access;
        void draw(draw_command& command);

    };

}
}