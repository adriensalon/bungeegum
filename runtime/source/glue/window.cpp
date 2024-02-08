#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/window.hpp>

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

#include <iostream>

namespace bungeegum {
namespace detail {

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    EM_JS(int, emscripten_get_canvas_width, (const char* canvas_id), {
        var canvas = document.getElementById(UTF8ToString(canvas_id));
        canvas.width = canvas.getBoundingClientRect().width;
        return canvas.getBoundingClientRect().width;
    });

    EM_JS(int, emscripten_get_canvas_height, (const char* canvas_id), {
        var canvas = document.getElementById(UTF8ToString(canvas_id));
        canvas.height = canvas.getBoundingClientRect().height;
        return canvas.getBoundingClientRect().height;
    });

    struct emscripten_loop_data {
        std::function<void(const BUNGEEGUM_USE_TIME_UNIT&)> callback;
        BUNGEEGUM_USE_TIME_UNIT target_frame_duration;
    };

    struct emscripten_data {
        std::vector<emscripten_key_event> key_events = {};
        std::vector<emscripten_mouse_event> mouse_events = {};
        std::vector<emscripten_wheel_event> wheel_events = {};
        std::vector<std::string> keys_down = {};
        std::vector<std::string> keys_up = {};
        std::vector<unsigned int> mouse_buttons_down = {};
        std::vector<unsigned int> mouse_buttons_up = {};
        float2 mouse_position = zero<float2>;
        float2 mouse_position_delta = zero<float2>;
        bool must_lock_cursor = false;
        bool must_unlock_cursor = false;
    };

    inline static emscripten_data emscripten_static_data = {};

    [[nodiscard]] std::string emscripten_result_to_string(EMSCRIPTEN_RESULT result)
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

    void emscripten_check_errors(EMSCRIPTEN_RESULT result)
    {
        if (result != EMSCRIPTEN_RESULT_SUCCESS) {
            std::string _error_message = emscripten_result_to_string(result);
            throw backtraced_exception(_error_message);
        }
    }

    void emscripten_cursor_lock()
    {
        if (emscripten_static_data.must_lock_cursor) {
            EmscriptenPointerlockChangeEvent _pointer_lock;
            emscripten_check_errors(emscripten_get_pointerlock_status(&_pointer_lock));
            if (!_pointer_lock.isActive) {
                emscripten_check_errors(emscripten_request_pointerlock("#canvas", 1));
            }
            emscripten_static_data.must_lock_cursor = false;
            // verify ptr is locked ?
        } else if (emscripten_static_data.must_unlock_cursor) {
            EmscriptenPointerlockChangeEvent _pointer_lock;
            emscripten_check_errors(emscripten_get_pointerlock_status(&_pointer_lock));
            if (_pointer_lock.isActive) {
                emscripten_check_errors(emscripten_exit_pointerlock());
            }
            emscripten_static_data.must_unlock_cursor = false;
            // verify ptr is unlocked ?
        }
    }

    [[nodiscard]] EM_BOOL key_callback(int event_type, const EmscriptenKeyboardEvent* event, void* user_data)
    {
        emscripten_static_data.key_events.push_back({ event_type, event });
        if (event_type == EMSCRIPTEN_EVENT_KEYDOWN) {
            emscripten_static_data.keys_down.push_back(event->key);
        } else if (event_type == EMSCRIPTEN_EVENT_KEYUP) {
            emscripten_static_data.keys_up.push_back(event->key);
        }
        return 0;
    }

    [[nodiscard]] EM_BOOL mouse_callback(int event_type, const EmscriptenMouseEvent* event, void* user_data)
    {
        emscripten_static_data.mouse_events.push_back({ event_type, event });

        // https://github.com/emscripten-core/emscripten/blob/main/test/test_html5_core.c
        if (event_type == EMSCRIPTEN_EVENT_MOUSEMOVE) {
            emscripten_static_data.mouse_position = float2 { event->clientX, event->clientY };
            emscripten_static_data.mouse_position_delta = float2 { event->movementX, event->movementY };
        }
        if (event_type == EMSCRIPTEN_EVENT_MOUSEDOWN) {
            unsigned int _button = event->button;
            emscripten_static_data.mouse_buttons_down.push_back(_button);
        } else if (event_type == EMSCRIPTEN_EVENT_MOUSEUP) {
            unsigned int _button = event->button;
            emscripten_static_data.mouse_buttons_up.push_back(_button);
        }
        if (event_type == EMSCRIPTEN_EVENT_CLICK) // we lock / unlock the pointer on click
        {
            emscripten_cursor_lock();
        } else if (event_type == EMSCRIPTEN_EVENT_MOUSEOVER) {
        } else if (event_type == EMSCRIPTEN_EVENT_MOUSEOUT) {
        }
        return 0;
    }

    [[nodiscard]] EM_BOOL touch_callback(int event_type, const EmscriptenTouchEvent* event, void* user_data)
    {
        const EmscriptenTouchPoint* _touch_point = &(event->touches[0]);
        float2 _touch_move_position = glm::vec2((float)_touch_point->clientX, (float)_touch_point->clientY);
        // if (event_type == EMSCRIPTEN_EVENT_TOUCHSTART) {
        // detail::mouse_position_delta = { 0, 0 };
        // } else if (event_type == EMSCRIPTEN_EVENT_TOUCHMOVE)
        //     detail::mouse_position_delta = _touch_move_position - detail::mouse_position;
        // detail::mouse_position = _touch_move_position;
        return EM_TRUE; // we use preventDefault() for touch callbacks (see Safari on iPad)
    }

    void emscripten_loop(void* window_ptr)
    {
        emscripten_loop_data* _typed_data = static_cast<emscripten_loop_data*>(window_ptr);
        _typed_data->callback(_typed_data->target_frame_duration);
    }

    [[nodiscard]] bool emscripten_setup()
    {
        emscripten_check_errors(emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback));
        emscripten_check_errors(emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback));
        emscripten_check_errors(emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback));
        emscripten_check_errors(emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_mouseenter_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_mouseleave_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_mouseover_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_mouseout_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback));
        emscripten_check_errors(emscripten_set_touchstart_callback("#canvas", nullptr, 1, touch_callback));
        // emscripten_assert(emscripten_set_touchend_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback));
        emscripten_check_errors(emscripten_set_touchmove_callback("#canvas", nullptr, 1, touch_callback)); // EMSCRIPTEN_EVENT_TARGET_WINDOW doesnt work on safari
        // emscripten_assert(emscripten_set_touchcancel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback));
        return true;
    }

    inline static bool emscripten_static_setup = emscripten_setup();
#else
    static void sdl_check_errors()
    {
        std::string _error_message = SDL_GetError();
        SDL_ClearError();
        if (!_error_message.empty()) {
            throw backtraced_exception(_error_message);
        }
    }

    static void sdl_check_main_ready()
    {
        static bool _is_sdl_main_ready = false;
        if (!_is_sdl_main_ready) {
            SDL_SetMainReady();
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
                sdl_check_errors();
            }
            _is_sdl_main_ready = true;
        }
    }
#endif

    window::window()
    {
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _sdl_events = std::make_shared<std::vector<SDL_Event>>();
#endif
    }

    window::~window()
    {
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        if (_sdl_window) {
            destroy_native();
        }
#endif
    }

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
	void attach_emscripten(const std::string& canvas_id)
	{
		_canvas_id = canvas_id;
	}
#else

    void window::attach_native(void* sdl_window)
    {
		if (_sdl_window) {
			// throw
		}
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
        SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, "1");
#endif
        _sdl_window = SDL_CreateWindowFrom(sdl_window);
        sdl_check_errors();
    }

    void window::attach_sdl(SDL_Window* sdl_window)
    {
		if (_sdl_window) {
			// throw
		}
        _sdl_window = sdl_window;
    }

    void window::create_native()
    {
		if (_sdl_window) {
			// throw
		}
        sdl_check_main_ready();
        SDL_WindowFlags _window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        _sdl_window = SDL_CreateWindow(
            default_native_window_title.data(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            static_cast<int>(math::floor(default_native_window_size.x)),
            static_cast<int>(math::floor(default_native_window_size.y)),
            _window_flags);
        sdl_check_errors();
    }

    void window::destroy_native()
    {
		if (!_sdl_window) {
            // throw
		}
		SDL_DestroyWindow(_sdl_window);
    }
#endif

    void window::cursor(const bool enabled)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        // TODO
#else
        if (SDL_ShowCursor(enabled) < 0) {
            sdl_check_errors();
        }
#endif
    }

    void window::fullscreen(const bool enabled)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        // TODO
#else
		if (!_sdl_window) {
            // throw
		}
        if (enabled && SDL_SetWindowFullscreen(_sdl_window, SDL_WINDOW_FULLSCREEN) < 0) {
            sdl_check_errors();
        } else if (SDL_SetWindowFullscreen(_sdl_window, NULL) < 0) {
            sdl_check_errors();
        }
#endif
    }

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    std::vector<emscripten_key_event>& window::get_emscripten_key_events()
    {
        return emscripten_static_data.key_events;
    }

    std::vector<emscripten_mouse_event>& window::get_emscripten_mouse_events()
    {
        return emscripten_static_data.mouse_events;
    }

    std::vector<emscripten_wheel_event>& window::get_emscripten_wheel_events()
    {
        return emscripten_static_data.wheel_events;
    }
#endif

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void* window::get_native() const
    {
        if constexpr (is_platform_win32 || is_platform_uwp) {
            SDL_SysWMinfo _wmi;
            SDL_VERSION(&_wmi.version);
            if (SDL_GetWindowWMInfo(_sdl_window, &_wmi) != SDL_TRUE) {
                sdl_check_errors();
            }
            return _wmi.info.win.window;
        } else if constexpr (is_platform_linux) {

        } else if constexpr (is_platform_macos) {
        }
    }

    SDL_Window* window::get_sdl() const
    {
        return _sdl_window;
    }

	std::vector<SDL_Event>& window::get_sdl_events()
	{
		return *_sdl_events;
	}

	bool window::has_native_window() const
	{		
		return _sdl_window;
	}
#endif

    float2 window::get_size() const
    {
        return _display_size;
    }

    void window::poll(const bool poll_device)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
		if (!_canvas_id.has_value()) {
			// throw
		}
        if (poll_device) {
            for (const unsigned int _button : emscripten_static_data.mouse_buttons_down) {
                mouse_down_event _mouse_down_event;
                _mouse_down_event.button_index = _button;
                mouse_down_events.push_back(_mouse_down_event);
            }
            emscripten_static_data.mouse_buttons_down.clear();
            for (const unsigned int _button : emscripten_static_data.mouse_buttons_up) {
                mouse_up_event _mouse_up_event;
                _mouse_up_event.button_index = _button;
                mouse_up_events.push_back(_mouse_up_event);
            }
            emscripten_static_data.mouse_buttons_up.clear();
            if (math::abs(emscripten_static_data.mouse_position_delta.x) > 0.f && math::abs(emscripten_static_data.mouse_position_delta.y) > 0.f) {
                mouse_moved_event _mouse_moved_event;
                _mouse_moved_event.absolute_position = emscripten_static_data.mouse_position;
                _mouse_moved_event.relative_position = emscripten_static_data.mouse_position_delta;
                mouse_moved_events.push_back(_mouse_moved_event);
            }
            emscripten_static_data.mouse_position_delta = zero<float2>;
            float2 _window_size_memory = _display_size;
			const char* _ccanvas_id = _canvas_id.value().c_str();
            _display_size = float2 { emscripten_get_canvas_width(_ccanvas_id), emscripten_get_canvas_height(_ccanvas_id) };
            if (_window_size_memory != _display_size) {
                window_resized_event _window_resized_event;
                _window_resized_event.new_size = _display_size;
                window_resized_events.push_back(_window_resized_event);
            }
        }
#else
		if (!_sdl_window) {
            // throw
		}
        SDL_Event _event;
        while (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_QUIT) {
                _is_running = false;
            } else if (_event.type == SDL_WINDOWEVENT && _event.window.event == SDL_WINDOWEVENT_CLOSE && _event.window.windowID == SDL_GetWindowID(_sdl_window)) {
                _is_running = false;
            } else {
                if (poll_device) {
                    switch (_event.type) {
                    case SDL_MOUSEBUTTONDOWN:
						mouse_down_event _mouse_down_event;
						_mouse_down_event.button_index = _event.button.button;
						mouse_down_events.push_back(_mouse_down_event);
                        break;
                    case SDL_MOUSEMOTION:
						mouse_moved_event _mouse_moved_event;
						_mouse_moved_event.absolute_position = { _event.motion.x, _event.motion.y };
						_mouse_moved_event.relative_position = { _event.motion.xrel, _event.motion.yrel };
						mouse_moved_events.push_back(_mouse_moved_event);
                        break;
                    case SDL_MOUSEBUTTONUP:
						mouse_up_event _mouse_up_event;
						_mouse_up_event.button_index = _event.button.button;
						mouse_up_events.push_back(_mouse_up_event);
                        break;

                        // todo

                    case SDL_WINDOWEVENT:
                        if (_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            window_resized_event _window_resized_event;
                            _window_resized_event.is_fullscreen = false;
                            _window_resized_event.new_size = { _event.window.data1, _event.window.data2 };
                            window_resized_events.push_back(_window_resized_event);
                        }
                        break;

                    default:
                        break;
                    }
                }
                _sdl_events->push_back(_event);
            }
        }
#endif
    }

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void window::resize(const float2 new_size)
    {
        // eventuellement sanitize to : max(0, round(new_size))
        if constexpr (is_platform_win32 || is_platform_uwp || is_platform_macos || is_platform_linux) {
            int _width = static_cast<int>(new_size.x);
            int _height = static_cast<int>(new_size.y);
            SDL_SetWindowSize(_sdl_window, _width, _height);
        }
    }

    void window::title(const std::string& description)
    {
        SDL_SetWindowTitle(_sdl_window, description.c_str());
    }
#endif

    void window::update_once(const std::optional<unsigned int> max_framerate, const std::function<void(const BUNGEEGUM_USE_TIME_UNIT&)>& update_callback)
    {
        BUNGEEGUM_USE_TIME_UNIT _target_frame_duration;
        if (max_framerate.has_value()) {
            BUNGEEGUM_USE_TIME_UNIT _one_second = std::chrono::duration_cast<BUNGEEGUM_USE_TIME_UNIT>(std::chrono::seconds(1));
            _target_frame_duration = BUNGEEGUM_USE_TIME_UNIT(static_cast<unsigned int>(math::floor(_one_second.count() / 60.f)));
        } else {
            _target_frame_duration = BUNGEEGUM_USE_TIME_UNIT(0);
        }
        BUNGEEGUM_USE_TIME_UNIT _delta_time = _loop_watch.lap_at_least(_target_frame_duration);
        update_callback(max_framerate.has_value() ? _target_frame_duration : _delta_time);
    }

    void window::update_loop(const std::optional<unsigned int> max_framerate, const std::function<void(const BUNGEEGUM_USE_TIME_UNIT&)>& update_callback)
    {
        BUNGEEGUM_USE_TIME_UNIT _target_frame_duration;
        if (max_framerate.has_value()) {
            BUNGEEGUM_USE_TIME_UNIT _one_second = std::chrono::duration_cast<BUNGEEGUM_USE_TIME_UNIT>(std::chrono::seconds(1));
            _target_frame_duration = BUNGEEGUM_USE_TIME_UNIT(static_cast<unsigned int>(math::floor(_one_second.count() / 60.f)));
        } else {
            _target_frame_duration = BUNGEEGUM_USE_TIME_UNIT(0);
        }
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        emscripten_loop_data _data { update_callback, _target_frame_duration };
        int _fps = max_framerate.has_value() ? max_framerate.value() : 0;
        emscripten_set_main_loop_arg(emscripten_loop, static_cast<void*>(&_data), _fps, EM_TRUE);
#else
        _is_running = true;
        while (_is_running) {
            BUNGEEGUM_USE_TIME_UNIT _delta_time = _loop_watch.lap_at_least(_target_frame_duration);
            update_callback(max_framerate.has_value() ? _target_frame_duration : _delta_time);
        }
#endif
    }

}
}