#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that describes this app in the operating system.
    struct Title {

        HOTSWAP_CLASS(Title);

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        Title& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        /// @brief The primary color to use for the application in the operating system interface.
        Title& color(const Color value);

        /// @brief A one-line description of this app for use in the window manager.
        Title& title(const std::string& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        const char* okok = "heyyyyaaa";

        std::optional<runtime_widget> _childWidget = std::nullopt;
    };

}
}