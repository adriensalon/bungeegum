#pragma once

#include <bungeegum/backend/standalone.hpp>
#include <bungeegum/core/runtime.hpp>

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
    return detail::viewport_size;
}

void launch(const runtime_widget& widget, const std::function<void()>& on_renderer_started)
{
    detail::protect_library([&]() {
        detail::global_widgets_manager.root() = detail::global_widgets_manager.raw(widget);
        detail::stopwatch _stopwatch;
        detail::window _window;
        detail::viewport_size = _window.get_size();
        detail::renderer _renderer(_window);

#if BUNGEEGUM_USE_HOTRELOAD
        if (!detail::reload_manager) {
            detail::reload_manager = std::make_unique<detail::reloader>();
        }
        for (std::filesystem::path& _include_directory : detail::reload_include_directories) {
            detail::reload_manager->add_include_directory(_include_directory);
        }
        for (std::filesystem::path& _library : detail::reload_libraries) {
            detail::reload_manager->add_library(_library);
        }
        for (std::filesystem::path& _source_directory : detail::reload_source_directories) {
            detail::reload_manager->add_source_directory(_source_directory);
        }
        for (std::filesystem::path& _force_compiled_source_file : detail::reload_force_compiled_source_files) {
            detail::reload_manager->add_force_compiled_source_file(_force_compiled_source_file);
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
            detail::context::mouse_down_events.push_back(event);
        });
        _window.on_mouse_moved([](const detail::mouse_moved_event& event) {
            detail::context::mouse_moved_events.push_back(event);
        });
        _window.on_mouse_pressed([](const detail::mouse_pressed_event& event) {
            detail::context::mouse_pressed_events.push_back(event);
        });
        _window.on_mouse_up([](const detail::mouse_up_event& event) {
            detail::context::mouse_up_events.push_back(event);
        });
        _window.on_window_resized([&_renderer](const detail::window_resized_event& event) {
            detail::context::window_resized_events.push_back(event);
            _renderer.process_window_resized_event(event);
        });
        _window.on_sdl_event([&_renderer](const SDL_Event* event) { // TOUJOURS CA MAIS FAUT REMOVE IMGUI PR CLEAN
            _renderer.process_sdl_event_for_imgui(event);
        });
        //
        //

        _window.on_update([&_window, &_renderer, &_stopwatch]() {
            detail::update_desired_data(_window);
            detail::viewport_size = _window.get_size();
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
            detail::reload_state _reload_result = detail::reload_manager->update();
            if (_reload_result == detail::reload_state::started_compiling) {
                detail::global_widgets_manager.save_widgets("C:/Users/adri/desktop/ok.json");
            } else if (_reload_result == detail::reload_state::performed_swap) {
                detail::global_widgets_manager.load_widgets("C:/Users/adri/desktop/ok.json");
            }
#endif
        });
        _renderer.clear_color = { 1.f, 1.f, 1.f, 1.f };
        _window.run_loop();
    });
}
}
