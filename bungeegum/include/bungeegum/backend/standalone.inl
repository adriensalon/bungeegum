#pragma once

#include <bungeegum/core/context.fwd>
#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/glue/renderer.fwd>
#include <bungeegum/glue/time.fwd>
#include <bungeegum/glue/window.fwd>

namespace bungeegum {

template <typename widget_t>
void launch(widget_t& widget, const std::function<void()>& on_renderer_started)
{
    // detail::protect_library([&]() {
    detail::widgets_context.build_root(widget);
    detail::stopwatch _stopwatch;
    detail::window _window;
    detail::renderer _renderer(_window);
    detail::setup_overlay();
    if (on_renderer_started)
        on_renderer_started();
    _renderer.rebuild_fonts();
    _window.on_input([&_renderer](const std::any& _event) {
        _renderer.process_input(_event);
    });
    _window.on_update([&_window, &_renderer, &_stopwatch]() {
        std::chrono::microseconds _max_fps_period_microseconds = std::chrono::microseconds(static_cast<unsigned int>(
            std::floorf(1000000.f / 60.f /* MAX FPS !!!*/)));
        std::chrono::microseconds _delta_time = _stopwatch.lap_at_least(_max_fps_period_microseconds);
        std::chrono::milliseconds _delta_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_max_fps_period_microseconds);
        // bool _has_thrown = has_userspace_thrown();
        bool _has_polled = _window.poll();
        if (!detail::has_userspace_thrown()) {
            // bool _has_polled = _window.poll();
            bool _has_ticked = detail::tick(_delta_milliseconds);
            if (_has_polled || _has_ticked) {
                _renderer.new_frame();
                detail::draw();
                _renderer.present();
            }
        } else {
            _renderer.new_frame();
            detail::draw();
            _renderer.present();
        }
    });
    _renderer.set_clear_color({ 1.f, 1.f, 1.f, 1.f });
    _window.run_loop();
    // });
}
}
