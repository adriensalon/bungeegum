#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/misc/Alignment.hpp>
#include <bungeegum/widgets/misc/Color.hpp>

namespace bungeegum {
namespace widgets {

    /// Immitates https://api.flutter.dev/flutter/widgets/Container-class.html
    struct Container {

        /// @brief The child contained by the container.
        template <typename child_widget_t>
        Container& child(child_widget_t& value)
        {
            adopt(this, value);
            return *this;
        }

        /// @brief The color to paint behind the child.
        /// @details This property should be preferred when the background is a simple color. For
        /// other cases, such as gradients or images, use the decoration property. If the
        /// decoration is used, this property will not.
        Container& color(const Color value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        void draw(draw_command& command);
        std::optional<adopted_widget> _adopted = std::nullopt;
        Color _color;
        float4x4 _transform;
        Alignment _alignment = Alignment::center();
        float1 _heightFactor = 1.f;
        float1 _widthFactor = 1.f;
    };

}
}