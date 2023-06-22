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
                abandon(this, _child.value());
            _child = adopt(this, value);
            return *this;
        }

        /// @brief The child will become as large as its parent allows.
        SizedBox& expand();

        /// @brief Requires the child to have exactly this height.
        SizedBox& height(const float1 value);

        /// @brief The child will become as small as its parent allows.
        SizedBox& shrink();

        /// @brief Requires the child to have exactly this size.
        SizedBox& size(const Size value);

        /// @brief Requires the child to have exactly this width and height.
        SizedBox& square(const float1 value);

        /// @brief Requires the child to have exactly this width.
        SizedBox& width(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _child = std::nullopt;
        float2 _size = zero<float2>;
    };

}
}