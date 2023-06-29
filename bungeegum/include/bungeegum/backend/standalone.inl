#pragma once

#include <bungeegum/context/context.fwd>
#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/glue/renderer.fwd>
#include <bungeegum/glue/time.fwd>
#include <bungeegum/glue/window.fwd>

namespace bungeegum {
namespace detail {

    inline bool is_desired_app_color_changed = false;
    inline float4 desired_app_color = { 1.f, 1.f, 1.f, 1.f };
    inline bool is_desired_app_title_changed = false;
    inline std::string desired_app_title = "Bungeegum App";

    void update_desired_data(window& current_window);
}

template <typename widget_t>
void launch(widget_t& widget, const std::function<void()>& on_renderer_started)
{
    detail::protect_library([&]() {
        detail::widgets_context.root = detail::get_untyped_widget(widget);
        detail::stopwatch _stopwatch;
        detail::window _window;
        detail::viewport_size = _window.get_size();
        detail::renderer _renderer(_window);
        //
        //
        // #if BUNGEEGUM_ENABLE_HOTRELOAD
        detail::reloader = std::make_unique<detail::reload_manager>();
        for (std::filesystem::path& _include_directory : detail::include_directories) {
            detail::reloader->add_include_directory(_include_directory);
        }
        for (std::filesystem::path& _source_directory : detail::include_directories) {
            detail::reloader->add_source_directory(_source_directory);
        }
        // #endif
        //
        //
        detail::setup_overlay();
        if (on_renderer_started)
            on_renderer_started();
        _renderer.rebuild_fonts();
        _window.on_input([&_renderer](const std::any& _event) {
            _renderer.process_input(_event);
            detail::process_input(_event);
        });
        _window.on_update([&_window, &_renderer, &_stopwatch]() {
            detail::update_desired_data(_window);
            detail::viewport_size = _window.get_size();
            // std::cout << "OK viewport size = " << detail::viewport_size.x << ", " << detail::viewport_size.y << std::endl;

            std::chrono::microseconds _max_fps_period_microseconds = std::chrono::microseconds(static_cast<unsigned int>(std::floorf(1000000.f / 60.f /* MAX FPS !!!*/)));
            std::chrono::microseconds _delta_time = _stopwatch.lap_at_least(_max_fps_period_microseconds);
            std::chrono::milliseconds _delta_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_max_fps_period_microseconds);
            bool _has_polled = _window.poll();
            bool _has_ticked = detail::tick(_delta_milliseconds);
            if (_has_ticked) {
                _renderer.new_frame();
                detail::draw(false);
                _renderer.present();
            }

            //
            //
            detail::reloader->update();
            //
            //
        });
        _renderer.set_clear_color({ 1.f, 1.f, 1.f, 1.f });
        _window.run_loop();
    });
}
}
