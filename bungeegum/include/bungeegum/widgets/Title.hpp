#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/misc/Color.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that describes this app in the operating system.
    struct Title {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        Title& child(child_widget_t& value)
        {
            adopt(this, value);
            return *this;
        }

        /// @brief The primary color to use for the application in the operating system interface.
        Title& color(const Color value);

        /// @brief A one-line description of this app for use in the window manager.
        Title& title(const std::string& value);
    };

}
}