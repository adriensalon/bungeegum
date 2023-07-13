#pragma once

#include <bungeegum/backend/standalone.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/process.fwd>
#include <bungeegum/core/runtime.hpp>
#include <bungeegum/glue/renderer.hpp>
#include <bungeegum/glue/time.hpp>

#if BUNGEEGUM_USE_STANDALONE

namespace bungeegum {
namespace detail {

    void standalone_manager::app_color(const float4 rgba_color)
    {
        _app_color = rgba_color;
    }

    void standalone_manager::app_title(const std::string& description)
    {
        _app_title = description;
    }

    void standalone_manager::update(window& current_window)
    {
        if (_app_color.has_value()) {
            // todo in window ?
            _app_color = std::nullopt;
        }
        if (_app_title.has_value()) {
            current_window.set_title(_app_title.value());
            _app_title = std::nullopt;
        }
    }
}

#endif

void standalone_app::color(const float4 rgba)
{
#if BUNGEEGUM_USE_STANDALONE
    detail::global_manager::standalone().app_color(rgba);
#endif
}

void standalone_app::title(const std::string& description)
{
#if BUNGEEGUM_USE_STANDALONE
    detail::global_manager::standalone().app_title(description);
#endif
}

float2 standalone_app::viewport()
{
#if BUNGEEGUM_USE_STANDALONE
    return detail::global_manager::backend().viewport_size;
#else
    return float2 {};
#endif
}

#if BUNGEEGUM_USE_STANDALONE

void launch(const runtime_widget& widget)
{
    detail::global_manager::logs().protect_library([&]() {
        detail::global_manager::widgets().root() = detail::global_manager::widgets().raw(widget);
        detail::stopwatch _stopwatch;
        detail::window _window;
        detail::global_manager::backend().viewport_size = _window.get_size();
        detail::renderer _renderer(_window);

#if BUNGEEGUM_USE_HOTSWAP
        detail::global_manager::backend().setup_if_required();
#endif

#if BUNGEEGUM_USE_OVERLAY
        detail::setup_overlay();
#endif

        _renderer.rebuild_fonts();

        //
        // web events
        _window.on_mouse_down([](const detail::mouse_down_event& event) {
            detail::global_manager::process().mouse_down_events.push_back(event);
        });
        _window.on_mouse_moved([](const detail::mouse_moved_event& event) {
            detail::global_manager::process().mouse_moved_events.push_back(event);
        });
        _window.on_mouse_pressed([](const detail::mouse_pressed_event& event) {
            detail::global_manager::process().mouse_pressed_events.push_back(event);
        });
        _window.on_mouse_up([](const detail::mouse_up_event& event) {
            detail::global_manager::process().mouse_up_events.push_back(event);
        });
        _window.on_window_resized([&_renderer](const detail::window_resized_event& event) {
            detail::global_manager::process().window_resized_events.push_back(event);
            _renderer.process_window_resized_event(event);
        });
        _window.on_sdl_event([&_renderer](const SDL_Event* event) { // TOUJOURS CA MAIS FAUT REMOVE IMGUI PR CLEAN
            _renderer.process_sdl_event_for_imgui(event);
        });
        //
        //

        _window.on_update([&_window, &_renderer, &_stopwatch]() {
            detail::global_manager::standalone().update(_window);
            detail::global_manager::backend().viewport_size = _window.get_size();
            // std::cout << "OK viewport size = " << detail::viewport_size.x << ", " << detail::viewport_size.y << std::endl;

            std::chrono::microseconds _max_fps_period_microseconds = std::chrono::microseconds(static_cast<unsigned int>(std::floorf(1000000.f / 60.f /* MAX FPS !!!*/)));
            std::chrono::microseconds _delta_time = _stopwatch.lap_at_least(_max_fps_period_microseconds);
            std::chrono::milliseconds _delta_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_max_fps_period_microseconds);
            bool _has_polled = _window.poll();
            (void)_has_polled;
            bool _has_ticked = detail::global_manager::process().update(_delta_milliseconds);
            if (_has_ticked) {
                _renderer.new_frame();
                detail::global_manager::process().render();
                _renderer.present();
            }

#if BUNGEEGUM_USE_HOTSWAP
            // FAIRE PAREIL AVANT / APRES FORCE UPDATE
            std::wstringstream _sstream;
            detail::reload_state _reload_result = detail::global_manager::backend().reload_manager->update(_sstream.rdbuf());
            if (_reload_result == detail::reload_state::started_compiling) {
                detail::global_manager::widgets().save_widgets("C:/Users/adri/desktop/ok.json");
            } else if (_reload_result == detail::reload_state::performed_swap) {
                detail::global_manager::widgets().load_widgets("C:/Users/adri/desktop/ok.json");
            }
#endif
        });
        _renderer.clear_color = { 1.f, 1.f, 1.f, 1.f };
        _window.run_loop();
    });
}

#endif
}
