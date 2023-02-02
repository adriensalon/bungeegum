//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <SDL.h>

#include <staticgui/glue/window.hpp>

namespace staticgui {
namespace glue {

    void launch_window(
        const std::string& initial_title,
        const unsigned int initial_width,
        const unsigned int initial_height,
        std::function<void()> start_callback,
        std::function<void()> update_callback,
        std::function<void()> end_callback)
    {
    }

    void enable_fullscreen()
    {
        SDL_SetWindowFullscreen(detail::sdl_window, SDL_WINDOW_FULLSCREEN);
    }

    void disable_fullscreen()
    {
        SDL_SetWindowFullscreen(detail::sdl_window, 0);
    }
}
}