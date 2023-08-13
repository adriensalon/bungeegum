#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief Used to configure the auto validation of FormField and Form widgets.
    enum struct AutoValidateMode {

		/// @brief No auto validation will occur.
		disabled,

		/// @brief Used to auto-validate Form and FormField even without user interaction.
		always,

		/// @brief Used to auto-validate Form and FormField only after each user interaction.
		onUserInteraction
    };

}
}