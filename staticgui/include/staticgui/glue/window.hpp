//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>
#include <functional>
#include <string>

#include <staticgui/glue/backtrace.hpp>

#if !defined(__EMSCRIPTEN__)
struct SDL_Window;
#endif

namespace staticgui {
namespace glue {

    struct window {
        window();
        ~window();
        window(const window& other) = delete;
        window& operator=(const window& other) = delete;
        window(window&& other);
        window& operator=(window&& other);

        SDL_Window* get_sdl_window() const;

        void set_title(const std::string& title);

        void set_size(const float width, const float height);

        void set_fullscreen(const bool enabled);

        void on_input(const std::function<void(const std::any&)>& event_callback);

        void on_update(const std::function<void()>& update_callback);

        void run_loop();

        static void show_cursor(const bool show);

#if !defined(__EMSCRIPTEN__)
        window(SDL_Window* sdl_window);
        window(void* native_window);
        void* get_native_window() const;
#endif

    private:
        std::function<void(const std::any&)> _event_callback = nullptr;
        std::function<void()> _update_callback = nullptr;
        std::any _window_impl = nullptr;
    };

}
}