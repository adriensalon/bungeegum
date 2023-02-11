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

#include <staticgui/glue/infinity.hpp>
#include <staticgui/glue/renderer.hpp>
#include <staticgui/glue/time.hpp>
#include <staticgui/glue/window.hpp>
#include <staticgui/state/context.hpp>
#include <staticgui/state/curve.hpp>

namespace staticgui {
namespace detail {

    struct host_state {

        template <typename widget_t>
        void launch(widget_t& widget, const std::function<void()>& on_renderer_started);

        template <typename widget_t>
        [[nodiscard]] std::function<void()> launch_embedded(widget_t& widget);

        context_state context;

    private:
        stopwatch _stopwatch;
        std::unique_ptr<window> _window = nullptr;
        std::unique_ptr<renderer> _renderer = nullptr;
    };

}
}

#include <staticgui/state/host.inl>