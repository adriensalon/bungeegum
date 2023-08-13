#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/MediaQueryData.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief 
    struct MediaQuery {

        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        MediaQuery& child(const runtime_widget& value);

		/// @brief Contains information about the current media.
		MediaQueryData& data() const;

    private:
		std::optional<runtime_widget> _child = std::nullopt;

    };

}
}