#pragma once

#include <bungeegum/backend/standalone.hpp>
#include <bungeegum/core/context.fwd>
#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/runtime.hpp>
#include <bungeegum/glue/renderer.hpp>
#include <bungeegum/glue/time.hpp>

namespace bungeegum {
namespace detail {

    void update_desired_data(window& current_window)
    {
        if (detail::is_desired_app_color_changed) {
            // todo bizarre
            detail::is_desired_app_color_changed = false;
        }
        if (detail::is_desired_app_title_changed) {
            current_window.set_title(desired_app_title);
            detail::is_desired_app_title_changed = false;
        }
    }
}

void bungeegum_app::color(const float4 rgba)
{
    detail::is_desired_app_color_changed = true;
    detail::desired_app_color = rgba;
}

void bungeegum_app::title(const std::string& description)
{
    detail::is_desired_app_title_changed = true;
    detail::desired_app_title = description;
}

float2 bungeegum_app::viewport()
{
    return detail::global_manager::backend().viewport_size;
}

void launch(const runtime_widget& widget, const std::function<void()>& on_renderer_started)
{
    detail::protect_library([&]() {
        detail::global_manager::widgets().root() = detail::global_manager::widgets().raw(widget);
        detail::stopwatch _stopwatch;
        detail::window _window;
        detail::global_manager::backend().viewport_size = _window.get_size();
        detail::renderer _renderer(_window);

#if BUNGEEGUM_USE_HOTRELOAD
        if (!detail::global_manager::backend().reload_manager) {
            detail::global_manager::backend().reload_manager = std::make_unique<detail::reloader>();
        }
        for (const std::string& _define : detail::global_manager::backend().reload_defines) {
            detail::global_manager::backend().reload_manager->add_define(_define);
        }
        for (const std::filesystem::path& _include_directory : detail::global_manager::backend().reload_include_directories) {
            detail::global_manager::backend().reload_manager->add_include_directory(_include_directory);
        }
        for (const std::filesystem::path& _library : detail::global_manager::backend().reload_libraries) {
            detail::global_manager::backend().reload_manager->add_library(_library);
        }
        for (const std::filesystem::path& _source_directory : detail::global_manager::backend().reload_source_directories) {
            detail::global_manager::backend().reload_manager->add_source_directory(_source_directory);
        }
        for (const std::filesystem::path& _force_compiled_source_file : detail::global_manager::backend().reload_force_compiled_source_files) {
            detail::global_manager::backend().reload_manager->add_force_compiled_source_file(_force_compiled_source_file);
        }
#endif

#if BUNGEEGUM_USE_OVERLAY
        detail::setup_overlay();
#endif

        if (on_renderer_started)
            on_renderer_started();
        _renderer.rebuild_fonts();

        //
        // web events
        _window.on_mouse_down([](const detail::mouse_down_event& event) {
            detail::process_manager::mouse_down_events.push_back(event);
        });
        _window.on_mouse_moved([](const detail::mouse_moved_event& event) {
            detail::process_manager::mouse_moved_events.push_back(event);
        });
        _window.on_mouse_pressed([](const detail::mouse_pressed_event& event) {
            detail::process_manager::mouse_pressed_events.push_back(event);
        });
        _window.on_mouse_up([](const detail::mouse_up_event& event) {
            detail::process_manager::mouse_up_events.push_back(event);
        });
        _window.on_window_resized([&_renderer](const detail::window_resized_event& event) {
            detail::process_manager::window_resized_events.push_back(event);
            _renderer.process_window_resized_event(event);
        });
        _window.on_sdl_event([&_renderer](const SDL_Event* event) { // TOUJOURS CA MAIS FAUT REMOVE IMGUI PR CLEAN
            _renderer.process_sdl_event_for_imgui(event);
        });
        //
        //

        _window.on_update([&_window, &_renderer, &_stopwatch]() {
            detail::update_desired_data(_window);
            detail::global_manager::backend().viewport_size = _window.get_size();
            // std::cout << "OK viewport size = " << detail::viewport_size.x << ", " << detail::viewport_size.y << std::endl;

            std::chrono::microseconds _max_fps_period_microseconds = std::chrono::microseconds(static_cast<unsigned int>(std::floorf(1000000.f / 60.f /* MAX FPS !!!*/)));
            std::chrono::microseconds _delta_time = _stopwatch.lap_at_least(_max_fps_period_microseconds);
            std::chrono::milliseconds _delta_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_max_fps_period_microseconds);
            bool _has_polled = _window.poll();
            (void)_has_polled;
            bool _has_ticked = detail::tick(_delta_milliseconds);
            if (_has_ticked) {
                _renderer.new_frame();
                detail::draw(false);
                _renderer.present();
            }

#if BUNGEEGUM_USE_HOTRELOAD
            // FAIRE PAREIL AVANT / APRES FORCE UPDATE
            detail::reload_state _reload_result = detail::global_manager::backend().reload_manager->update();
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
}
