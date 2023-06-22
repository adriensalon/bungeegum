#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that describes this app in the operating system.
    struct Title {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        Title& child(child_widget_t& value)
        {
            if (_child.has_value())
                abandon(this, _child.value());
            _child = runtime_widget(value);
            adopt(this, _child.value());
            return *this;
        }

        /// @brief The primary color to use for the application in the operating system interface.
        Title& color(const Color value);

        /// @brief A one-line description of this app for use in the window manager.
        Title& title(const std::string& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _child = std::nullopt;
    };

}
}