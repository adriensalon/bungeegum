#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief
    struct AnimatedContainer {

        /// @brief
        AnimatedContainer& child(const std::optional<runtime_widget>& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
    };

}
}