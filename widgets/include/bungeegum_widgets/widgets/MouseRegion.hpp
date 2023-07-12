#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/SystemMouseCursor.hpp>

namespace bungeegum {
namespace widgets {

    struct MouseRegion {

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        MouseRegion& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        MouseRegion& cursor(const SystemMouseCursor value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
    };

}
}