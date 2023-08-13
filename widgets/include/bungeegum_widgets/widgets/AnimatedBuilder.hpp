#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A general-purpose widget for building animations.
    /// @details AnimatedBuilder is useful for more complex widgets that wish to include an
    /// animation as part of a larger build function. To use AnimatedBuilder, construct the widget
    /// and pass it a builder function. For simple cases without additional state, consider using
    /// AnimatedWidget.
    struct AnimatedBuilder {

        /// @brief A builder that builds a widget given a child.
        using TransitionBuilder = std::function<runtime_widget(const runtime_widget& child)>;

        /// @brief
		template <typename animatedValue_t>
        AnimatedBuilder& animation(const animation<animatedValue_t>& value);

        /// @brief
        AnimatedBuilder& builder(const TransitionBuilder& value);

        /// @brief The widget below this widget in the tree.
        AnimatedAlign& child(const std::optional<runtime_widget>& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        animation<Alignment> _animatedAlignment = {};
        float2 _sizeFactor = { 1.f, 1.f };
    };

}
}