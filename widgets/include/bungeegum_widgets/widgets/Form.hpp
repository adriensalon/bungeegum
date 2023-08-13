#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/AutoValidateMode.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief
    struct Form {

		/// @brief Signature for a callback that verifies that it's OK to call Navigator.pop.
		using WillPopCallback = std::function<std::future<bool>()>;

		/// @brief Used to enable/disable form fields auto validation and update their error text. 
		Form& autovalidateMode(const AutoValidateMode value);
	
        /// @brief The widget below this widget in the tree.
        /// @details This widget can only have one child. To lay out multiple children, let this
        /// widget's child be a widget such as Row, Column, or Stack, which have a children
        /// property, and then provide the children to that widget.
        Form& child(const runtime_widget& value);

		/// @brief Called when one of the form fields changes. 
		Form& onChanged(const std::function<void()>& value);

		/// @brief Enables the form to veto attempts by the user to dismiss the ModalRoute that contains the form. 
		Form& onWillPop(const WillPopCallback& value);




    private:
    };

}
}