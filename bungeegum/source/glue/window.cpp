#include <bungeegum/glue/backtrace.fwd>
#include <bungeegum/glue/toolchain.fwd>
#include <bungeegum/glue/window.fwd>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#else
#include <SDL.h>
#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
#include <SDL_syswm.h>
#endif
#endif

namespace bungeegum {
namespace detail {

    struct window::window_data {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

        //
        //
        // TODO
        //
        //

#else
        SDL_Window* sdl_window = nullptr;
        std::function<void(const SDL_Event*)> sdl_event_callback = nullptr;
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
#endif
    };

    window::window()
    {
        if constexpr (is_platform_emscripten_v) {
            static bool1 _is_already_created = false;
            if (_is_already_created) {
                throw backtraced_exception("", backtrace_size);
            }
            _is_already_created = true;

            //
            //
            // TODO
            //
            //

        } else {
            if (!window_data::is_sdl_main_ready) {
                SDL_SetMainReady();
                if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
                    window_data::check_errors();
                }
                window_data::is_sdl_main_ready = true;
            }
            _data = std::make_shared<window_data>();
            SDL_WindowFlags _window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
            _data->sdl_window = SDL_CreateWindow(
                window_data::sdl_default_window_title.data(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                window_data::sdl_default_window_size.x,
                window_data::sdl_default_window_size.y,
                _window_flags);
            window_data::check_errors();
        }
    }

    window::~window()
    {
        if constexpr (is_platform_emscripten_v) {

            //
            //
            // TODO
            //
            //

        } else {
            SDL_DestroyWindow(_data->sdl_window);
            window_data::check_errors();
        }
    }

    window::window(void* native_window)
    {
        static_assert(!is_platform_emscripten_v, "An instance can only be created from an raw pointer to an OS window on "
                                                 "platforms where SDL2 is supported such as Windows, MacOS, Linux, iOS, Android.");
        (void)native_window;

        //
        //
        // TODO
        //
        //
    }

    window::window(SDL_Window* sdl_window)
    {
        static_assert(!is_platform_emscripten_v, "An instance can only be created from an SDL_Window on platforms "
                                                 "where SDL2 is supported such as Windows, MacOS, Linux, iOS, Android.");
        window_data::is_sdl_main_ready = true;
        _data->sdl_window = sdl_window;
    }

    void* window::get_native_window() const
    {
        static_assert(!is_platform_emscripten_v, "A raw pointer to an OS window can only be retrieved on platforms "
                                                 "where SDL2 is supported such as Windows, MacOS, Linux, iOS, Android.");
        if constexpr (is_platform_win32_v || is_platform_uwp_v) {
            SDL_SysWMinfo _wmi;
            SDL_VERSION(&_wmi.version);
            if (SDL_GetWindowWMInfo(_data->sdl_window, &_wmi) != SDL_TRUE) {
                window_data::check_errors();
            }
            return _wmi.info.win.window;
        } else if constexpr (is_platform_linux_v) {

        } else if constexpr (is_platform_macos_v) {
        }
    }

    SDL_Window* window::get_sdl_window() const
    {
        static_assert(!is_platform_emscripten_v, "A raw pointer to an SDL_Window can only be retrieved on platforms "
                                                 "where SDL2 is supported such as Windows, MacOS, Linux, iOS, Android.");
        return _data->sdl_window;
    }

    uint2 window::get_size() const
    {
        int _width, _height;
        SDL_GetWindowSize(_data->sdl_window, &_width, &_height);
        return float2 { static_cast<uint1>(_width), static_cast<uint1>(_height) };
    }

    void window::on_update(const std::function<void()>& update_callback)
    {
        _update_callback = update_callback;
    }

    void window::on_mouse_down(const std::function<void(const mouse_down_event&)>& mouse_down_callback)
    {
        _mouse_down_callback = mouse_down_callback;
    }

    void window::on_mouse_moved(const std::function<void(const mouse_moved_event&)>& mouse_moved_callback)
    {
        _mouse_moved_callback = mouse_moved_callback;
    }

    void window::on_mouse_pressed(const std::function<void(const mouse_pressed_event&)>& mouse_pressed_callback)
    {
        _mouse_pressed_callback = mouse_pressed_callback;
    }

    void window::on_mouse_up(const std::function<void(const mouse_up_event&)>& mouse_up_callback)
    {
        _mouse_up_callback = mouse_up_callback;
    }

    void window::on_sdl_event(const std::function<void(const SDL_Event*)>& sdl_event_callback)
    {
        static_assert(!is_platform_emscripten_v, "A callback taking an SDL_Event can only be stored on platforms "
                                                 "where SDL2 is supported such as Windows, MacOS, Linux, iOS, Android.");
        _data->sdl_event_callback = sdl_event_callback;
    }

    void window::on_window_resized(const std::function<void(const window_resized_event&)>& window_resized_callback)
    {
        _window_resized_callback = window_resized_callback;
    }

    bool window::poll()
    {
        SDL_Event _event;
        bool _polled = false;
        while (SDL_PollEvent(&_event)) {
            _polled = true;
            if (_event.type == SDL_QUIT)
                _is_running = false;
            else if (_event.type == SDL_WINDOWEVENT && _event.window.event == SDL_WINDOWEVENT_CLOSE && _event.window.windowID == SDL_GetWindowID(_data->sdl_window))
                _is_running = false;
            else {
                if constexpr (!is_platform_emscripten_v) {
                    if (_data->sdl_event_callback) {
                        const SDL_Event* _event_ptr = &_event;
                        _data->sdl_event_callback(_event_ptr);
                    }
                }
                switch (_event.type) {
                case SDL_MOUSEMOTION:
                    mouse_moved_event _mouse_moved_event;
                    _mouse_moved_event.absolute_position = { _event.motion.x, _event.motion.y };
                    _mouse_moved_event.relative_position = { _event.motion.xrel, _event.motion.yrel };
                    if (_mouse_moved_callback) {
                        _mouse_moved_callback(_mouse_moved_event);
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if (_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        window_resized_event _window_resized_event;
                        _window_resized_event.is_fullscreen = false;
                        _window_resized_event.new_size = { _event.window.data1, _event.window.data2 };
                        if (_window_resized_callback) {
                            _window_resized_callback(_window_resized_event);
                        }
                    }
                    break;

                    //
                    //
                    // TODO
                    //
                    //

                default:
                    break;
                }
            }
        }
        return _polled;
    }

    void window::run_loop()
    {
        if constexpr (is_platform_emscripten_v) {

            //
            //
            // TODO
            //
            //

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

            //
            //
            // TODO
            //
            //

        } else {
            if (enabled && SDL_SetWindowFullscreen(_data->sdl_window, SDL_WINDOW_FULLSCREEN) < 0) {
                window_data::check_errors();
            } else if (SDL_SetWindowFullscreen(_data->sdl_window, NULL) < 0) {
                window_data::check_errors();
            }
        }
    }

    void window::set_size(const uint2 size)
    {
        if constexpr (is_platform_emscripten_v) {

            //
            //
            // TODO
            //
            //

        } else {
            int _width = static_cast<int>(size.x);
            int _height = static_cast<int>(size.y);
            SDL_SetWindowSize(_data->sdl_window, _width, _height);
        }
    }

    void window::set_title(const std::string& title)
    {
        if constexpr (is_platform_emscripten_v) {

            //
            //
            // TODO
            //
            //

        } else {
            SDL_SetWindowTitle(_data->sdl_window, title.c_str());
        }
    }

    void window::show_cursor(const bool enabled)
    {
        if constexpr (is_platform_emscripten_v) {

            //
            //
            // TODO
            //
            //

        } else {
            if (SDL_ShowCursor(enabled) < 0) {
                window_data::check_errors();
            }
        }
    }
}
}