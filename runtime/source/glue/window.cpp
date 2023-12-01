#include <bungeegum/glue/backtrace.fwd>
#include <bungeegum/glue/window.fwd>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#else
#include <SDL.h>
#endif

#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
#include <SDL_syswm.h>
#include <windows.h>
#endif

namespace bungeegum {
namespace detail {

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    EM_JS(int, canvas_get_width, (), {
        var canvas = document.getElementById('canvas');
        canvas.width = canvas.getBoundingClientRect().width;
        return canvas.getBoundingClientRect().width;
    });

    EM_JS(int, canvas_get_height, (), {
        var canvas = document.getElementById('canvas');
        canvas.height = canvas.getBoundingClientRect().height;
        return canvas.getBoundingClientRect().height;
    });
#endif

    struct window::window_data {
        float2 window_size = { 0, 0 };
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        bool must_lock_cursor = false;
        bool must_unlock_cursor = false;
        std::vector<std::string> keys_down = {};
        std::vector<std::string> keys_up = {};
        std::vector<unsigned int> mouse_buttons_down = {};
        std::vector<unsigned int> mouse_buttons_up = {};
        float2 mouse_position_delta = { 0.f, 0.f };

        [[nodiscard]] static std::string result_to_string(EMSCRIPTEN_RESULT result)
        {
            if (result == EMSCRIPTEN_RESULT_SUCCESS)
                return "EMSCRIPTEN_RESULT_SUCCESS";
            if (result == EMSCRIPTEN_RESULT_DEFERRED)
                return "EMSCRIPTEN_RESULT_DEFERRED";
            if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED)
                return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
            if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED)
                return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
            if (result == EMSCRIPTEN_RESULT_INVALID_TARGET)
                return "EMSCRIPTEN_RESULT_INVALID_TARGET";
            if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET)
                return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
            if (result == EMSCRIPTEN_RESULT_INVALID_PARAM)
                return "EMSCRIPTEN_RESULT_INVALID_PARAM";
            if (result == EMSCRIPTEN_RESULT_FAILED)
                return "EMSCRIPTEN_RESULT_FAILED";
            if (result == EMSCRIPTEN_RESULT_NO_DATA)
                return "EMSCRIPTEN_RESULT_NO_DATA";
            return "Unknown EMSCRIPTEN_RESULT!";
        }

        static void check_errors(EMSCRIPTEN_RESULT result)
        {
            if (result != EMSCRIPTEN_RESULT_SUCCESS) {
                std::string _error_message = result_to_string(result);
                throw backtraced_exception(_error_message);
            }
        }

        // https://github.com/emscripten-core/emscripten/blob/main/test/test_html5_core.c

        static void update_cursor_lock()
        {
            if (must_lock_cursor) {
                EmscriptenPointerlockChangeEvent _pointer_lock;
                check_errors(emscripten_get_pointerlock_status(&_pointer_lock));
                if (!_pointer_lock.isActive) {
                    check_errors(emscripten_request_pointerlock("#canvas", 1));
                }
                must_lock_cursor = false;
                // verify ptr is locked ?
            } else if (must_unlock_cursor) {
                EmscriptenPointerlockChangeEvent _pointer_lock;
                check_errors(emscripten_get_pointerlock_status(&_pointer_lock));
                if (_pointer_lock.isActive) {
                    check_errors(emscripten_exit_pointerlock());
                }
                must_unlock_cursor = false;
                // verify ptr is unlocked ?
            }
        }

        [[nodiscard]] static EM_BOOL key_callback(int event_type, const EmscriptenKeyboardEvent* event, void* user_data)
        {
            if (event_type == EMSCRIPTEN_EVENT_KEYDOWN) {
                keys_down.emplace_back(event->key);
            } else if (event_type == EMSCRIPTEN_EVENT_KEYUP) {
                keys_up.emplace_back(event->key);
            }
            return 0;
        }

        [[nodiscard]] static EM_BOOL mouse_callback(int event_type, const EmscriptenMouseEvent* event, void* user_data)
        {
            // https://github.com/emscripten-core/emscripten/blob/main/test/test_html5_core.c
            if (event_type == EMSCRIPTEN_EVENT_MOUSEMOVE) {
                // detail::mouse_position_delta = glm::vec2((float)event->movementX, (float)event->movementY);
                // detail::mouse_position = glm::vec2((float)event->clientX, (float)event->clientY);
            }
            if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN) {
                unsigned int _button = event->button;
                detail::buttons[_button] = true;
                detail::buttons_changed.emplace_back(_button);
            } else if (event_type == EMSCRIPTEN_EVENT_MOUSEUP) {
                unsigned int _button = event->button;
                // detail::buttons[_button] = false;
                // detail::buttons_changed.emplace_back(_button);
            }
            if (event_type == EMSCRIPTEN_EVENT_CLICK) // we lock / unlock the pointer on click
            {
                update_cursor_lock();
            } else if (event_type == EMSCRIPTEN_EVENT_MOUSEOVER) {
            } else if (event_type == EMSCRIPTEN_EVENT_MOUSEOUT) {
            }
            return 0;
        }

        [[nodiscard]] static EM_BOOL touch_callback(int event_type, const EmscriptenTouchEvent* event, void* user_data)
        {
            const EmscriptenTouchPoint* _touch_point = &(event->touches[0]);
            glm::vec2 _touch_move_position = glm::vec2((float)_touch_point->clientX, (float)_touch_point->clientY);
            // if (event_type == EMSCRIPTEN_EVENT_TOUCHSTART) {
            // detail::mouse_position_delta = { 0, 0 };
            // } else if (event_type == EMSCRIPTEN_EVENT_TOUCHMOVE)
            //     detail::mouse_position_delta = _touch_move_position - detail::mouse_position;
            // detail::mouse_position = _touch_move_position;
            return EM_TRUE; // we use preventDefault() for touch callbacks (see Safari on iPad)
        }

        [[nodiscard]] static bool setup_emscripten()
        {
            check_errors(emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback));
            check_errors(emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback));
            check_errors(emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback));
            check_errors(emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_mouseenter_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_mouseleave_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_mouseover_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_mouseout_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
            check_errors(emscripten_set_touchstart_callback("#canvas", nullptr, 1, touch_callback));
            // emscripten_assert(emscripten_set_touchend_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback));
            check_errors(emscripten_set_touchmove_callback("#canvas", nullptr, 1, touch_callback)); // EMSCRIPTEN_EVENT_TARGET_WINDOW doesnt work on safari
            // emscripten_assert(emscripten_set_touchcancel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback));
            return true;
        }

        inline static bool static_setup = setup_emscripten();
#else
        SDL_Window* sdl_window = nullptr;
        std::function<void(const SDL_Event*)> sdl_event_callback = nullptr;
        inline static std::string_view sdl_default_window_title = "bungeegum v0.0";
        inline static int2 sdl_default_window_size = { 1280, 720 };

        static void check_errors()
        {
            std::string _error_message = SDL_GetError();
            SDL_ClearError();
            if (!_error_message.empty()) {
                throw backtraced_exception(_error_message);
            }
        }

        static void check_main_ready()
        {
            static bool is_sdl_main_ready = false;
            if (!is_sdl_main_ready) {
                SDL_SetMainReady();
                if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
                    check_errors();
                }
                is_sdl_main_ready = true;
            }
        }
#endif
        static void check_only_one()
        {
            static bool _is_already_created = false;
            if (_is_already_created) {
                throw backtraced_exception("Only one window can be created on this platform.");
            }
            _is_already_created = true;
        }
    };

    window::~window()
    {
        if constexpr (is_platform_emscripten) {
            // TODO
        } else {
            // no go destroy()
            // SDL_DestroyWindow(_data->sdl_window);
            // window_data::check_errors();
        }
    }

    void window::create()
    {
        _data = std::make_shared<window_data>();
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        window_data::check_only_one();
        // TODO
#else
        window_data::check_main_ready();
        SDL_WindowFlags _window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        _data->sdl_window = SDL_CreateWindow(
            window_data::sdl_default_window_title.data(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_data::sdl_default_window_size.x,
            window_data::sdl_default_window_size.y,
            _window_flags);
        window_data::check_errors();
#endif
    }

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void window::attach_sdl(SDL_Window* sdl_window)
    {
        _data = std::make_shared<window_data>();
        _data->sdl_window = sdl_window;
        window_data::check_errors();
    }

    void window::attach_native(void* native_window)
    {
        _data = std::make_shared<window_data>();
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
        SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, "1");
#endif
        _data->sdl_window = SDL_CreateWindowFrom(native_window);
        window_data::check_errors();
    }
#endif

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void window::attach_emscripten()
    {
        // TODO
    }
#endif

    void window::cursor(const bool enabled)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        // TODO
#else
        if (SDL_ShowCursor(enabled) < 0) {
            window_data::check_errors();
        }
#endif
    }

    void window::fullscreen(const bool enabled)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        // TODO
#else
        if (enabled && SDL_SetWindowFullscreen(_data->sdl_window, SDL_WINDOW_FULLSCREEN) < 0) {
            window_data::check_errors();
        } else if (SDL_SetWindowFullscreen(_data->sdl_window, NULL) < 0) {
            window_data::check_errors();
        }
#endif
    }

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void* window::get_native() const
    {
        if constexpr (is_platform_win32 || is_platform_uwp) {
            SDL_SysWMinfo _wmi;
            SDL_VERSION(&_wmi.version);
            if (SDL_GetWindowWMInfo(_data->sdl_window, &_wmi) != SDL_TRUE) {
                window_data::check_errors();
            }
            return _wmi.info.win.window;
        } else if constexpr (is_platform_linux) {

        } else if constexpr (is_platform_macos) {
        }
    }

    SDL_Window* window::get_sdl() const
    {
        return _data->sdl_window;
    }
#endif

    float2 window::get_size() const
    {
		return _data->window_size;
        // if constexpr (is_platform_emscripten) {
        //     // TODO
        // } else {
        //     int _width, _height;
        //     SDL_GetWindowSize(_data->sdl_window, &_width, &_height);
        //     return float2 { _width, _height };
        // }
    }

    void window::on_exit(const std::function<void()>& exit_callback)
    {
        _exit_callbacks.push_back(exit_callback);
    }

    void window::on_update(const std::function<void()>& update_callback)
    {
        _update_callbacks.push_back(update_callback);
    }

    void window::on_mouse_down(const std::function<void(const mouse_down_event&)>& mouse_down_callback)
    {
        _mouse_down_events.callbacks.push_back(mouse_down_callback);
    }

    void window::on_mouse_moved(const std::function<void(const mouse_moved_event&)>& mouse_moved_callback)
    {
        _mouse_moved_events.callbacks.push_back(mouse_moved_callback);
    }

    void window::on_mouse_pressed(const std::function<void(const mouse_pressed_event&)>& mouse_pressed_callback)
    {
        _mouse_pressed_events.callbacks.push_back(mouse_pressed_callback);
    }

    void window::on_mouse_up(const std::function<void(const mouse_up_event&)>& mouse_up_callback)
    {
        _mouse_up_events.callbacks.push_back(mouse_up_callback);
    }

    void window::on_resized(const std::function<void(const window_resized_event&)>& window_resized_callback)
    {
        _resized_events.callbacks.push_back(window_resized_callback);
    }

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void window::on_sdl_event(const std::function<void(const SDL_Event*)>& sdl_event_callback)
    {
        _data->sdl_event_callback = sdl_event_callback;
    }
#endif

    bool window::poll(const bool poll_simulated, const bool poll_device)
    {
        bool _polled = false;
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _data->window_size = float2 { canvas_get_width(), canvas_get_height() };
#else
        // poll device
        if constexpr (!is_platform_emscripten) { // emscripten events already stashed
            SDL_Event _event;
            while (SDL_PollEvent(&_event)) {
                _polled = true;
                if (_event.type == SDL_QUIT) {
                    _is_running = false;
                } else if (_event.type == SDL_WINDOWEVENT && _event.window.event == SDL_WINDOWEVENT_CLOSE && _event.window.windowID == SDL_GetWindowID(_data->sdl_window)) {
                    _is_running = false;
                } else {
                    if (_data->sdl_event_callback) {
                        const SDL_Event* _event_ptr = &_event;
                        _data->sdl_event_callback(_event_ptr);
                    }
                    if (poll_device) {
                        switch (_event.type) {
                        case SDL_MOUSEMOTION:
                            if (poll_device) {
                                mouse_moved_event _mouse_moved_event;
                                _mouse_moved_event.absolute_position = { _event.motion.x, _event.motion.y };
                                _mouse_moved_event.relative_position = { _event.motion.xrel, _event.motion.yrel };
                                _mouse_moved_events.stashed.push_back(_mouse_moved_event);
                            }
                            break;

                            // todo

                        case SDL_WINDOWEVENT:
                            if (_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                window_resized_event _window_resized_event;
                                _window_resized_event.is_fullscreen = false;
                                _window_resized_event.new_size = { _event.window.data1, _event.window.data2 };
                                _resized_events.stashed.push_back(_window_resized_event);
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
#endif

        // poll simulated
        if (poll_simulated) {
            _mouse_down_events.stashed.insert(
                _mouse_down_events.stashed.end(),
                _mouse_down_events.simulated.begin(),
                _mouse_down_events.simulated.end());
            _mouse_moved_events.stashed.insert(
                _mouse_moved_events.stashed.end(),
                _mouse_moved_events.simulated.begin(),
                _mouse_moved_events.simulated.end());
            _mouse_up_events.stashed.insert(
                _mouse_up_events.stashed.end(),
                _mouse_up_events.simulated.begin(),
                _mouse_up_events.simulated.end());
            _mouse_pressed_events.stashed.insert(
                _mouse_pressed_events.stashed.end(),
                _mouse_pressed_events.simulated.begin(),
                _mouse_pressed_events.simulated.end());
            _resized_events.stashed.insert(
                _resized_events.stashed.end(),
                _resized_events.simulated.begin(),
                _resized_events.simulated.end());
            _mouse_down_events.simulated.clear();
            _mouse_moved_events.simulated.clear();
            _mouse_up_events.simulated.clear();
            _mouse_pressed_events.simulated.clear();
            _resized_events.simulated.clear();
        }

        // fire callbacks
        for (const std::function<void(const mouse_down_event&)>& _callback : _mouse_down_events.callbacks) {
            for (const mouse_down_event& _event : _mouse_down_events.stashed) {
                _callback(_event);
            }
        }
        // if (_mouse_moved_events.callback) {
        //     for (const mouse_moved_event& _event : _mouse_moved_events.stashed) {
        //         _mouse_moved_events.callback(_event);
        //     }
        // }
        // if (_mouse_up_events.callback) {
        //     for (const mouse_up_event& _event : _mouse_up_events.stashed) {
        //         _mouse_up_events.callback(_event);
        //     }
        // }
        // if (_mouse_pressed_events.callback) {
        //     for (const mouse_pressed_event& _event : _mouse_pressed_events.stashed) {
        //         _mouse_pressed_events.callback(_event);
        //     }
        // }
        // if (_resized_events.callback) { // only the last resize is relevant
        //     _resized_events.callback(_resized_events.stashed.back());
        // }
        _mouse_down_events.stashed.clear();
        _mouse_moved_events.stashed.clear();
        _mouse_up_events.stashed.clear();
        _resized_events.stashed.clear();

        return _polled;
    }

    void window::update_once()
    {
        for (const std::function<void()>& _callback : _update_callbacks) {
            _callback();
        }
    }

    void window::update_loop(const std::optional<unsigned int> max_framerate)
    {
        (void)max_framerate;
        if constexpr (is_platform_emscripten) {
            // TODO
        } else {
            _is_running = true;
            while (_is_running) {
                update_once();
            }
        }
    }

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void window::resize(const float2 new_size)
    {
        // eventuellement sanitize to : max(0, round(new_size))
        if constexpr (is_platform_win32 || is_platform_uwp || is_platform_macos || is_platform_linux) {
            int _width = static_cast<int>(new_size.x);
            int _height = static_cast<int>(new_size.y);
            SDL_SetWindowSize(_data->sdl_window, _width, _height);
        }
    }
#endif

    void window::simulate_mouse_down(const unsigned int button_index)
    {
        mouse_down_event _event;
        _event.button_index = math::max(0u, button_index);
        _mouse_down_events.stashed.push_back(_event);
    }

    void window::simulate_mouse_move(const float2 absolute_position)
    {
        float2 _sanitized = math::max(zero<float2>, absolute_position);
        // _mouse_moved_events.push_back(_sanitized);
    }

    void window::simulate_mouse_move_relative(const float2 relative_position)
    {
        float2 _sanitized = math::max(zero<float2>, relative_position);
        // _mouse_moved_relative_events.push_back(_sanitized);
    }

    void window::simulate_mouse_up(const unsigned int button_index)
    {
        mouse_up_event _event;
        _event.button_index = math::max(0u, button_index);
        _mouse_up_events.stashed.push_back(_event);
    }

    void window::simulate_mouse_pressed(const unsigned int button_index)
    {
        mouse_pressed_event _event;
        _event.button_index = math::max(0u, button_index);
        _mouse_pressed_events.stashed.push_back(_event);
    }

    void window::title(const std::string& description)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

        // TODO
#else
        SDL_SetWindowTitle(_data->sdl_window, description.c_str());
#endif
    }
}
}