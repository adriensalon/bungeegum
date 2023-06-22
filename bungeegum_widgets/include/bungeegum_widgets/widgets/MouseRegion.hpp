#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/SystemMouseCursor.hpp>

namespace bungeegum {
namespace widgets {

    struct MouseRegion {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        MouseRegion& child(child_widget_t& value)
        {
            if (_child.has_value())
                abandon(this, _child.value());
            _child = adopt(this, value);
            return *this;
        }

        MouseRegion& cursor(const SystemMouseCursor value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _child = std::nullopt;
    };

}
}