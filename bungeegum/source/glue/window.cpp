#include <bungeegum/glue/backtrace.fwd>
#include <bungeegum/glue/toolchain.fwd>
#include <bungeegum/glue/window.fwd>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#else
#include <SDL.h>
#include <SDL_syswm.h>
#endif

namespace bungeegum {
namespace detail {

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    struct window::web_window_data {
    };
#else
    struct window::native_window_data {
        SDL_Window* sdl_window = nullptr;
        inline static bool is_sdl_main_ready = false;
        inline static std::string_view sdl_default_window_title = "bungeegum v0.0";
        inline static int2 sdl_default_window_size = { 1280, 720 };

        static void check_errors()
        {
            std::string _error_message = SDL_GetError();
            SDL_ClearError();
            if (!_error_message.empty()) {
                throw backtraced_exception(_error_message, detail::window::backtrace_size);
            }
        }
    };
#endif

    window::window()
    {
        if constexpr (is_platform_emscripten_v) {
        } else {
            if (!native_window_data::is_sdl_main_ready) {
                SDL_SetMainReady();
                if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
                    native_window_data::check_errors();
                }
                native_window_data::is_sdl_main_ready = true;
            }
            _data = std::make_shared<native_window_data>();
            SDL_WindowFlags _window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
            _data->sdl_window = SDL_CreateWindow(
                native_window_data::sdl_default_window_title.data(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                native_window_data::sdl_default_window_size.x,
                native_window_data::sdl_default_window_size.y,
                _window_flags);
            native_window_data::check_errors();
        }
    }

    window::~window()
    {
        if constexpr (is_platform_emscripten_v) {
        } else {
            SDL_DestroyWindow(_data->sdl_window);
            native_window_data::check_errors();
        }
    }

    window::window(void* native_window)
    {
        static_assert(!is_platform_emscripten_v);
        (void)native_window;
    }

    window::window(SDL_Window* sdl_window)
    {
        static_assert(!is_platform_emscripten_v);
        native_window_data::is_sdl_main_ready = true;
        _data->sdl_window = sdl_window;
    }

    void* window::get_native_window() const
    {
        static_assert(!is_platform_emscripten_v);
        if constexpr (is_platform_win32_v || is_platform_uwp_v) {
            SDL_SysWMinfo _wmi;
            SDL_VERSION(&_wmi.version);
            if (SDL_GetWindowWMInfo(_data->sdl_window, &_wmi) != SDL_TRUE) {
                native_window_data::check_errors();
            }
            return _wmi.info.win.window;
        } else if constexpr (is_platform_linux_v) {

        } else if constexpr (is_platform_macos_v) {
        }
    }

    SDL_Window* window::get_sdl_window() const
    {
        static_assert(!is_platform_emscripten_v);
        return _data->sdl_window;
    }

    uint2 window::get_size() const
    {
        int _width, _height;
        SDL_GetWindowSize(_data->sdl_window, &_width, &_height);
        return float2 { static_cast<uint1>(_width), static_cast<uint1>(_height) };
    }

    void window::on_event(const std::function<void(const std::any&)>& event_callback)
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
            else if (_event.type == SDL_WINDOWEVENT && _event.window.event == SDL_WINDOWEVENT_CLOSE && _event.window.windowID == SDL_GetWindowID(_data->sdl_window))
                _is_running = false;
            else
                _event_callback(_untyped);
        }
        return _polled;
    }

    void window::run_loop()
    {
        if constexpr (is_platform_emscripten_v) {
        } else {
            _is_running = true;
            while (_is_running) {
                _update_callback();
            }
        }
    }

    void window::set_fullscreen(const bool enabled)
    {
        if constexpr (is_platform_emscripten_v) {
        } else {
            if (enabled && SDL_SetWindowFullscreen(_data->sdl_window, SDL_WINDOW_FULLSCREEN) < 0) {
                native_window_data::check_errors();
            } else if (SDL_SetWindowFullscreen(_data->sdl_window, NULL) < 0) {
                native_window_data::check_errors();
            }
        }
    }

    void window::set_size(const uint2 size)
    {
        if constexpr (is_platform_emscripten_v) {
        } else {
            int _width = static_cast<int>(size.x);
            int _height = static_cast<int>(size.y);
            SDL_SetWindowSize(_data->sdl_window, _width, _height);
        }
    }

    void window::set_title(const std::string& title)
    {
        if constexpr (is_platform_emscripten_v) {
        } else {
            SDL_SetWindowTitle(_data->sdl_window, title.c_str());
        }
    }

    void window::show_cursor(const bool enabled)
    {
        if constexpr (is_platform_emscripten_v) {
        } else {
            if (SDL_ShowCursor(enabled) < 0) {
                native_window_data::check_errors();
            }
        }
    }
}
}