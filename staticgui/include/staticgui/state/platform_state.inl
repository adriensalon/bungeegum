//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <imgui.h>

namespace staticgui {
namespace detail {

    template <typename widget_t>
    void platform_state::launch(widget_t& widget)
    {
        _context.make_at_root(widget);
        _window = std::make_unique<glue::window>();
        _renderer = std::make_unique<glue::renderer>(*(_window.get()));
        _window->on_input([this](const std::any& _event) {
            _renderer->process_input(_event);
        });
        _window->on_update([this]() {
            _renderer->new_frame();

            ///
            ///
            ///
            ImGui::ShowDemoWindow();
            ///
            ///
            ///

            _renderer->present();
        });
        _renderer->set_clear_color({ .82f, .22f, .22f });
        _window->run_loop();
    }

    template <typename widget_t>
    void platform_state::attach(widget_t& widget)
    {
    }

}
}
