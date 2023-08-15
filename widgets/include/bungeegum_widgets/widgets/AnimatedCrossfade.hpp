#pragma once

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>
#include <bungeegum_widgets/core/CrossFadeState.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that cross-fades between two given children and animates itself between 
	/// their sizes.
	/// @details The animation is controlled through the crossFadeState parameter. firstCurve and
	/// secondCurve represent the opacity curves of the two children. The firstCurve is inverted,
	/// i.e. it fades out when providing a growing curve like Curves.linear. The sizeCurve is the
	/// curve used to animate between the size of the fading-out child and the size of the 
	/// fading-in child.
    struct AnimatedCrossFade {

		/// @brief Signature for the AnimatedCrossFade.layoutBuilder callback.
		/// @details The topChild is the child fading in, which is normally drawn on top. The 
		/// bottomChild is the child fading out, normally drawn on the bottom. For good performance
		/// the returned widget tree should contain both the topChild and the bottomChild; the
		/// depth of the tree, and the types of the widgets in the tree, from the returned widget 
		/// to each of the children should be the same.
		using AnimatedCrossFadeBuilder = std::function<runtime_widget(const runtime_widget& topChild, const runtime_widget& bottomChild)>;

		/// @brief Default implementation for layoutBuilder. 
		runtime_widget defaultLayoutBuilder(const runtime_widget& topChild, const runtime_widget& bottomChild);

		/// @brief How the children should be aligned while the size is animating.
		AnimatedCrossFade& alignment(const Alignment& value);

		/// @brief The child that will be shown when the animation has completed.
		AnimatedCrossFade& crossFadeState(const CrossFadeState value);

		/// @brief The duration of the whole orchestrated animation.
		/// @tparam durationUnit_t is the unit from std::chrono to use.
        template <typename durationUnit_t = std::chrono::seconds>
        AnimatedCrossFade& duration(const unsigned int value)
        {
            _animation.duration<durationUnit_t>(value);
            return *this;
        }

        /// @brief
        AnimatedCrossFade& excludeBottomFocus(const bool value);

        /// @brief
        AnimatedCrossFade& firstChild(const runtime_widget& value);

		AnimatedCrossFade firstCurve(const curve& value);

		/// @brief A builder that positions the firstChild and secondChild widgets.
		/// @details The widget returned by this method is wrapped in an AnimatedSize. By default, 
		/// this uses a Stack and aligns the bottomChild to the top of the stack while providing 
		/// the topChild as the non-positioned child to fill the provided constraints. This works 
		/// well when the AnimatedCrossFade is in a position to change size and when the children 
		/// are not flexible. However, if the children are less fussy about their sizes (for
		/// example a CircularProgressIndicator inside a Center), or if the AnimatedCrossFade is
		/// being forced to a particular size, then it can result in the widgets jumping about when
		/// the cross-fade state is changed.
		AnimatedCrossFade layoutBuilder(const AnimatedCrossFadeBuilder& value);

		/// @brief The duration of the whole orchestrated animation when running in reverse.
		/// @tparam durationUnit_t is the unit from std::chrono to use.
        template <typename durationUnit_t = std::chrono::seconds>
        AnimatedCrossFade& reverseDuration(const unsigned int value)
        {
			// TODODODO
            // _animation.duration<durationUnit_t>(value);
            return *this;
        }

        /// @brief The child that is visible when crossFadeState is CrossFadeState.showSecond. It
		/// fades in when transitioning crossFadeState from CrossFadeState.showFirst to
		/// CrossFadeState.showSecond and vice versa.
        AnimatedCrossFade& secondChild(const runtime_widget& value);

        /// @brief The fade curve of the second child.
		AnimatedCrossFade secondCurve(const curve& value);

		/// @brief  
		AnimatedCrossFade sizeCurve(const curve& value);


    private:
        friend struct access;
        void resolve(resolve_command& command);
        void draw(resolve_command& command);

		AnimatedCrossFadeBuilder _layoutBuilder = defaultLayoutBuilder;
		CrossFadeState _crossFadeState = CrossFadeState::showFirst;
		bool _excludeBottomFocus = false;
		animation<Alignment> _firstAlign = {};
		animation<Alignment> _secondAlign = {};
		animation<float1> _firstOpacity = {};
		animation<float1> _secondOpacity = {};
        std::optional<runtime_widget> _firstChild = std::nullopt;
        std::optional<runtime_widget> _secondChild = std::nullopt;
    };

}
}