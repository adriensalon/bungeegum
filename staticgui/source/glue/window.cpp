//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <future>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#else
#include <SDL.h>
#include <SDL_syswm.h>
#endif

#include <staticgui/glue/toolchain.hpp>
#include <staticgui/glue/window.hpp>

namespace staticgui {
namespace detail {

    namespace {

#if defined(__EMSCRIPTEN__)
#else
        static bool is_sdl_main_ready = false;

        SDL_Window* get_window(std::any& untyped)
        {
            return (std::any_cast<SDL_Window*>(untyped));
        }

        SDL_Window* get_window(const std::any& untyped)
        {
            return (std::any_cast<SDL_Window*>(untyped));
        }

        SDL_Event& get_event(std::any& untyped)
        {
            return (std::any_cast<SDL_Event&>(untyped));
        }
#endif
    }

    window::window()
    {
        if constexpr (is_platform_emscripten) {
        } else {
            if (!is_sdl_main_ready) {
                SDL_SetMainReady();
                SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER); //, == 0)
                detail::is_sdl_main_ready = true;
            }
            SDL_WindowFlags _window_flags = static_cast<SDL_WindowFlags>(
                SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
            _window_impl = SDL_CreateWindow("staticgui v0.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, _window_flags);
        }
    }

    window::~window()
    {
        if constexpr (is_platform_emscripten) {
        } else {
            SDL_DestroyWindow(detail::get_window(_window_impl));
        }
    }

    window::window(window&& other)
    {
    }

    window& window::operator=(window&& other)
    {
        return *this;
    }

    SDL_Window* window::get_sdl_window() const
    {
        return get_window(_window_impl);
    }

    void window::set_title(const std::string& title)
    {
        if constexpr (is_platform_emscripten) {
        } else {
            SDL_SetWindowTitle(detail::get_window(_window_impl), title.c_str());
        }
    }

    void window::set_size(const float width, const float height)
    {
        if constexpr (is_platform_emscripten) {
        } else {
            int _width = static_cast<int>(std::floorf(width));
            int _height = static_cast<int>(std::floorf(height));
            SDL_SetWindowSize(detail::get_window(_window_impl), _width, _height);
        }
    }

    void window::set_fullscreen(const bool enabled)
    {
        if constexpr (is_platform_emscripten) {
        } else {
            if (enabled)
                SDL_SetWindowFullscreen(detail::get_window(_window_impl), SDL_WINDOW_FULLSCREEN);
            else
                SDL_SetWindowFullscreen(detail::get_window(_window_impl), NULL);
        }
    }

    void window::on_input(const std::function<void(const std::any&)>& event_callback)
    {
        _event_callback = event_callback;
    }

    void window::on_update(const std::function<void()>& update_callback)
    {
        _update_callback = update_callback;
    }

    bool window::poll()
    {
        SDL_Event _event;
        bool _polled = false;
        while (SDL_PollEvent(&_event)) {
            _polled = true;
            std::any _untyped = _event;
            if (_event.type == SDL_QUIT)
                _is_running = false;
            else if (_event.type == SDL_WINDOWEVENT && _event.window.event == SDL_WINDOWEVENT_CLOSE && _event.window.windowID == SDL_GetWindowID(detail::get_window(_window_impl)))
                _is_running = false;
            else
                _event_callback(_untyped);
        }
        return _polled;
    }

    void window::run_loop()
    {
        if constexpr (is_platform_emscripten) {
        } else {
            _is_running = true;
            while (_is_running) {
                _update_callback();
            }
        }
    }

    void window::show_cursor(const bool enabled)
    {
        if constexpr (is_platform_emscripten) {
        } else {
            SDL_ShowCursor(enabled);
        }
    }

#if !defined(__EMSCRIPTEN__)
    window::window(SDL_Window* sdl_window)
    {
        detail::is_sdl_main_ready = true;
        _window_impl = sdl_window;
    }

    window::window(void* native_window)
    {
    }

    void* window::get_native_window() const
    {
        if constexpr (is_platform_win32 || is_platform_uwp) {
            SDL_SysWMinfo _wmi;
            SDL_VERSION(&_wmi.version);
            SDL_GetWindowWMInfo(detail::get_window(_window_impl), &_wmi); //, == SDL_TRUE)
            return _wmi.info.win.window;
        } else if constexpr (is_platform_linux) {

        } else if constexpr (is_platform_macos) {
        }
        return nullptr;
    }
#endif

}
}