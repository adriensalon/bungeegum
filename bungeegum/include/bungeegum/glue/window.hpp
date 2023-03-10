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

#include <bungeegum/glue/simd.hpp>

struct SDL_Window;

namespace bungeegum {
namespace detail {

    struct window {
        window();
        window(void* native_window);
        window(SDL_Window* sdl_window);
        window(const window& other) = delete;
        window& operator=(const window& other) = delete;
        window(window&& other);
        window& operator=(window&& other);
        ~window();

        [[nodiscard]] void* get_native_window() const;
        [[nodiscard]] SDL_Window* get_sdl_window() const;
        void set_title(const std::string& title);
        void set_size(const float2 size);
        void set_fullscreen(const bool enabled);
        void on_input(const std::function<void(const std::any&)>& event_callback);
        void on_update(const std::function<void()>& update_callback);
        void run_loop();
        bool poll();

        static void show_cursor(const bool show);

    private:
        bool _is_running = false;
        std::function<void(const std::any&)> _event_callback = nullptr;
        std::function<void()> _update_callback = nullptr;
        std::any _sdl_window = nullptr;
    };

}
}
