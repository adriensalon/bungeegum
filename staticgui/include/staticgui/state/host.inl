//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {

    template <typename widget_t>
    void host_state::launch(widget_t& widget)
    {
        protect_library([&]() {
            this->context.widgets.declare_root(widget);
            _window = std::make_unique<glue::window>();
            _renderer = std::make_unique<glue::renderer>(*(_window.get()));
            _window->on_input([this](const std::any& _event) {
                _renderer->process_input(_event);
            });
            _window->on_update([this]() {
                std::chrono::milliseconds _delta_time = _stopwatch.lap();
                float _delta_milliseconds = static_cast<float>(_delta_time.count());
                bool _has_thrown = has_userspace_thrown();
                if (!has_userspace_thrown()) {
                    if (this->context.tick(_delta_milliseconds)) {
                        _renderer->new_frame();
                        this->context.draw();
                        _renderer->present();
                    }
                } else {
                    _renderer->new_frame();
                    this->context.draw();
                    _renderer->present();
                }
            });
            _renderer->set_clear_color({ 1.f, 1.f, 1.f });
            _window->run_loop();
        });
    }

    template <typename widget_t>
    std::function<void()> host_state::attach(widget_t& widget)
    {
        protect_library([&]() {
            context.widgets.declare_root(widget);
        });
        return [this]() {
            protect_library([&]() {
                std::chrono::milliseconds _delta_time = _stopwatch.lap();
                float _delta_milliseconds = static_cast<float>(_delta_time.count());
                if (!has_userspace_thrown())
                    context.tick(_delta_milliseconds);
                context.draw();
            });
        }
    }

}
}
