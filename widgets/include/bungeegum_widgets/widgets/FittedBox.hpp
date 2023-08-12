#pragma once

#include <bungeegum_widgets/core/Alignment.hpp>
#include <bungeegum_widgets/core/BoxFit.hpp>

namespace bungeegum {
namespace widgets {

    struct FittedBox {

        /// @brief How to align the child within its parent's bounds.
        FittedBox& alignment(const Alignment value);

        /// @brief The widget below this widget in the tree.
        FittedBox& child(const runtime_widget& value);

        /// @brief The content will be clipped (or not) according to this option.
        FittedBox& clipBehavior(const Clip value);

        /// @brief How to inscribe the child into the space allocated during layout.
        FittedBox& fit(const BoxFit value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        // std::optional<runtime_widget> _childWidget = std::nullopt;
        BoxFit _fit = BoxFit::contain;
        Clip _clipBehavior = Clip::none;
        Alignment _alignment = Alignment::center();
    };

}
}