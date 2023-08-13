#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief Animated version of Align which automatically transitions the child's position over
	/// a given duration whenever the given alignment changes.
    struct AnimatedAlign {

        /// @brief How to align the child.
        AnimatedAlign& alignment(const Alignment& value);

        /// @brief The widget below this widget in the tree.
        AnimatedAlign& child(const std::optional<runtime_widget>& value);

		/// @brief The duration over which to animate the parameters of this container. 
		template <typename durationUnit_t = std::chrono::seconds>
    	AnimatedAlign& duration(const unsigned int value)
		{
			_animatedAlignment.duration<durationUnit_t>(value);
		}

        /// @brief Sets its height to the child's height multiplied by this factor.
        AnimatedAlign& heightFactor(const std::optional<double> value);

        /// @brief Called every time an animation completes.
        AnimatedAlign& onEnd(const std::function<void()>& value);

        /// @brief The curve to apply when animating the parameters of this container.
		AnimatedAlign& setCurve(const curve& value);

        /// @brief Sets its width to the child's width multiplied by this factor.
        AnimatedAlign& widthFactor(const std::optional<double> value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
		
        std::optional<runtime_widget> _childWidget = std::nullopt;
        animation<Alignment> _animatedAlignment = {};
        float2 _sizeFactor = { 1.f, 1.f };
    };

}
}