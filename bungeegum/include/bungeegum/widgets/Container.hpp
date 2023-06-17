#pragma once

//

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/misc/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// Immitates https://api.flutter.dev/flutter/widgets/Container-class.html
    struct Container {

        /// @brief The widget below this widget in the tree.
        /// @tparam child_widget_t
        /// @param value
        template <typename child_widget_t>
        Center& child(child_widget_t& value)
        {
            adopt(this, value);
            return *this;
        }

        /// @brief Sets its height to the child's height multiplied by this factor.
        /// @param value
        Center& heightFactor(const float1 value);

        /// @brief Sets its width to the child's width multiplied by this factor.
        /// @param value
        Center& widthFactor(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        std::optional<adopted_widget> _adopted = std::nullopt;
        float3 _color = { 1.f, 1.f, 1.f };
        float4x4 _transform;
        Alignment _alignment = Alignment::center();
        float1 _heightFactor = 1.f;
        float1 _widthFactor = 1.f;
    };

}
}