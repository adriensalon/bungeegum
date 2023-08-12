#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that attempts to size the child to a specific aspect ratio.
    /// @details The aspect ratio is expressed as a ratio of width to height. For example, a 16:9
    /// width:height aspect ratio would have a value of 16.f/9.f.
    struct AspectRatio {

        /// @brief The aspect ratio to attempt to use.
        AspectRatio& aspectRatio(const float1 value);

        /// @brief The widget below this widget in the tree.
        AspectRatio& child(const runtime_widget& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<runtime_widget> _childWidget = std::nullopt;
        std::optional<float1> _aspectRatio = std::nullopt;
    };

}
}