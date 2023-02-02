//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {
namespace detail {

    template <typename widget_t>
    void host_state::launch(widget_t& widget)
    {
        _context.build_root(widget);
        _window = std::make_unique<glue::window>();
        _renderer = std::make_unique<glue::renderer>(*(_window.get()));
        _window->on_input([this](const std::any& _event) {
            _renderer->process_input(_event);
        });
        _window->on_update([this]() {
            // measure elapsed time with window
            if (_context.tick(0.2f)) {
                _renderer->new_frame();
                _context.draw();
                _renderer->present();
            }
        });
        _renderer->set_clear_color({ .62f, .22f, .22f });
        _window->run_loop();
    }

    template <typename widget_t>
    std::function<void()> host_state::attach(widget_t& widget)
    {
        _context.build_root(widget);
        return [this]() {
            // measure elapsed time with window
            _context.tick(0.2f);
            _context.draw();
        }
    }

}
}
