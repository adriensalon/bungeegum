#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that imposes additional constraints on its child.
    struct SizedBox {

        /// @brief The widget below this widget in the tree.
        template <typename child_widget_t>
        SizedBox& child(child_widget_t& value)
        {
            if (_child.has_value())
                abandon(this, _child);
            _child = adopt(this, value);
            return *this;
        }

        SizedBox& height(const float1 value);

        SizedBox& width(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<adopted_widget> _child = std::nullopt;
        float2 _size = infinity<float2>; // ig
    };

}
}