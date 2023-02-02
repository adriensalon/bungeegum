//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <memory>

#include <staticgui/glue/renderer.hpp>
#include <staticgui/glue/window.hpp>
#include <staticgui/state/context.hpp>

namespace staticgui {
namespace detail {

    struct host_state {

        template <typename widget_t>
        void launch(widget_t& widget);

        template <typename widget_t>
        [[nodiscard]] std::function<void()> attach(widget_t& widget);

    private:
        context_state _context;
        std::unique_ptr<glue::window> _window = nullptr;
        std::unique_ptr<glue::renderer> _renderer = nullptr;
    };

}
}

#include <staticgui/state/host.inl>