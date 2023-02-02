//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/renderer.hpp>
#include <staticgui/glue/window.hpp>
#include <staticgui/state/context_state.hpp>
#include <staticgui/state/layout_state.hpp>

namespace staticgui {
namespace detail {

    struct platform_state {

        template <typename widget_t>
        void launch(widget_t& widget);

        template <typename widget_t>
        void attach(widget_t& widget);

    private:
        context_state _context;
        layout_state _layout;
        // window
        // renderer
    };

}
}

#include <staticgui/state/platform_state.inl>