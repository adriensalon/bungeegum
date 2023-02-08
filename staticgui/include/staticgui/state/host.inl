//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/overlay/overlay.hpp>
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
            overlay::install_font();
            _renderer->rebuild_fonts();
            _window->on_input([this](const std::any& _event) {
                _renderer->process_input(_event);
            });
            _window->on_update([this]() {
                std::chrono::microseconds _max_fps_period_microseconds = std::chrono::microseconds(static_cast<unsigned int>(std::floorf(1000000.f / context.max_fps)));
                std::chrono::microseconds _delta_time = _stopwatch.lap_at_least(_max_fps_period_microseconds);
                float _delta_milliseconds = 0.001f * static_cast<float>(_max_fps_period_microseconds.count());
                bool _has_thrown = has_userspace_thrown();
                if (!has_userspace_thrown()) {
                    bool _has_polled = _window->poll();
                    bool _has_ticked = this->context.tick(_delta_milliseconds);
                    if (_has_polled || _has_ticked) {
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
    std::function<void()> host_state::launch_embedded(widget_t& widget)
    {
        protect_library([&]() {
            overlay::install_font();
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
