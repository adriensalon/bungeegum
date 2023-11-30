#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Clip.hpp>
#include <bungeegum_widgets/core/CrossAxisAlignment.hpp>
#include <bungeegum_widgets/core/MainAxisAlignment.hpp>
#include <bungeegum_widgets/core/VerticalDirection.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that displays its children in a horizontal array.
	/// @details To cause a child to expand to fill the available horizontal space, wrap the child
	/// in an Expanded widget. The Row widget does not scroll (and in general it is considered an
	/// error to have more children in a Row than will fit in the available room). If you have a 
	/// line of widgets and want them to be able to scroll if there is insufficient room, consider
	/// using a ListView. For a vertical variant, see Column. If you only have one child, then
	/// consider using Align or Center to position the child. Layout for a Row proceeds in six 
	/// steps:
	/// - 	Layout each child with a null or zero flex factor (e.g., those that are not 
	///		Expanded) with unbounded horizontal constraints and the incoming vertical 
	///		constraints. If the crossAxisAlignment is CrossAxisAlignment.stretch, instead use 
	///		tight vertical constraints that match the incoming max height.
	/// -	Divide the remaining horizontal space among the children with non-zero flex factors 
	///		(e.g., those that are Expanded) according to their flex factor. For example, a child 
	///		with a flex factor of 2.0 will receive twice the amount of horizontal space as a child 
	///		with a flex factor of 1.0.
	/// -	Layout each of the remaining children with the same vertical constraints as in step 1, 
	///		but instead of using unbounded horizontal constraints, use horizontal constraints based
	///		 on the amount of space allocated in step 2. Children with Flexible.fit properties that 
	///		are FlexFit.tight are given tight constraints (i.e., forced to fill the allocated 
	///		space), and children with Flexible.fit properties that are FlexFit.loose are given 
	///		loose constraints (i.e., not forced to fill the allocated space).
	/// -	The height of the Row is the maximum height of the children (which will always satisfy 
	///		the incoming vertical constraints).
	/// -	The width of the Row is determined by the mainAxisSize property. If the mainAxisSize 
	///		property is MainAxisSize.max, then the width of the Row is the max width of the
	///		incoming constraints. If the mainAxisSize property is MainAxisSize.min, then the width
	///		of the Row is the sum of widths of the children (subject to the incoming constraints).
	/// -	Determine the position for each child according to the mainAxisAlignment and the 
	///		crossAxisAlignment. For example, if the mainAxisAlignment is 
	///		MainAxisAlignment.spaceBetween, any horizontal space that has not been allocated to 
	///		children is divided evenly and placed between the children.
    struct Row {

		/// @brief The widgets below this widget in the tree.
		Row& children(const std::vector<runtime_widget>& value);

		/// @brief The content will be clipped (or not) according to this option.
		Row& clipBehavior(const Clip value);

		/// @brief How the children should be placed along the cross axis.
		Row& crossAxisAlignment(const CrossAxisAlignment value);

		/// @brief How the children should be placed along the main axis. 
		Row& mainAxisAlignment(const MainAxisAlignment value);

		// text baseline

		// text direction

		/// @brief Determines the order to lay children out vertically and how to interpret start
		/// and end in the vertical direction.
		Row& verticalDirection(const VerticalDirection value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

		std::vector<runtime_widget> _children = {};
		Clip _clipBehavior = Clip::none;
		CrossAxisAlignment _crossAxisAlignment = CrossAxisAlignment::center;
		MainAxisAlignment _mainAxisAlignment = MainAxisAlignment::center;
    };

}
}