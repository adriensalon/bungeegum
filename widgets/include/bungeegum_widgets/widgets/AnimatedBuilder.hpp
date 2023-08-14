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

        /// @brief A builder that builds a widget given the current child.
        using TransitionBuilder = std::function<runtime_widget(const runtime_widget& child)>;

        /// @brief The existing animation that is part of a larger build function.
		template <typename animatedValue_t>
        AnimatedBuilder& animation(animation<animatedValue_t>& value)
		{
			value.on_value_changed([this] (const animatedValue_t& animatedValue) {
				(void)animatedValue;
				if (_builder.has_value() && _childWidget.has_value()) {
					_builder.value()(_childWidget.value());
				}
			})
		}

        /// @brief Called every time the animation notifies about a change.
        AnimatedBuilder& builder(const TransitionBuilder& value);

        /// @brief The child widget to pass to the builder.
		/// @details If the builder function contains a subtree that does not depend on the 
		/// listenable, it is more efficient to build that subtree once instead of rebuilding it on
		/// every change of the listenable. Performance is therefore improved by specifying any 
		/// widgets that don't need to change using the prebuilt child attribute. The 
		/// AnimatedBuilder passes this child back to the builder callback so that it can be 
		/// incorporated into the build. Using this pre-built child is entirely optional, but can 
		/// improve performance significantly in some cases and is therefore a good practice.
        AnimatedAlign& child(const std::optional<runtime_widget>& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<TransitionBuilder> _builder = std::nullopt;
        std::optional<runtime_widget> _childWidget = std::nullopt;
        animation<Alignment> _animatedAlignment = {};
        float2 _sizeFactor = { 1.f, 1.f };
    };

}
}