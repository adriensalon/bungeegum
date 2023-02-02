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
#include <optional>
#include <string>

typedef union SDL_Event SDL_Event;
struct SDL_Window;

namespace staticgui {
namespace glue {

    struct window {
        window();
        ~window();
#if !defined(__EMSCRIPTEN__)
        window(SDL_Window* sdl_window);
        window(void* native_window);
#endif
        window(const window& other) = delete;
        window& operator=(const window& other) = delete;
        window(window&& other);
        window& operator=(window&& other);

        void set_title(const std::string& title);

        void set_size(const float width, const float height);

        void set_fullscreen(const bool enabled);

#if !defined(__EMSCRIPTEN__)
        void* get_native_window();
#endif

        void on_event(const std::function<void(const std::any&)>& event_callback);

        void on_update(const std::function<void()>& update_callback);

        void run_loop();

        static void show_cursor(const bool show);

    private:
        std::function<void(const std::any&)> _event_callback = nullptr;
        std::function<void()> _update_callback = nullptr;
        std::any _impl = nullptr;
    };

}
}