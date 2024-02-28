#pragma once

#include <bungeegum/config/misc.hpp>
#include <bungeegum/core/math.hpp>
#include <bungeegum/glue/dialog.hpp>
#include <bungeegum/glue/time.hpp>
#include <bungeegum/glue/toolchain.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
struct EmscriptenMouseEvent;
struct EmscriptenWheelEvent;
struct EmscriptenKeyboardEvent;
#else
struct SDL_Window;
union SDL_Event;
#endif

namespace bungeegum {
namespace detail {

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

    /// @brief
    struct emscripten_key_event {
        int32_t event_type;
        const EmscriptenKeyboardEvent* event;
    };

    /// @brief
    struct emscripten_mouse_event {
        int32_t event_type;
        const EmscriptenMouseEvent* event;
    };

    /// @brief
    struct emscripten_wheel_event {
        int32_t event_type;
        const EmscriptenWheelEvent* event;
    };

#endif

    /// @brief Instances of this struct represent cross-platform window resized event data.
    struct window_resized_event {
        bool is_fullscreen;
        float2 new_size;
    };

    /// @brief Instances of this struct represent cross-platform mouse moved event data.
    struct mouse_moved_event {
        float2 absolute_position;
        float2 relative_position;
    };

    /// @brief Instances of this struct represent cross-platform mouse down event data.
    struct mouse_down_event {
        unsigned int button_index;
    };

    /// @brief Instances of this struct represent cross-platform mouse up event data.
    struct mouse_up_event {
        unsigned int button_index;
    };

    /// @brief Instances of this struct represent cross-platform mouse pressed event data.
    struct mouse_pressed_event {
        unsigned int button_index;
    };

    /// @brief Instances of this struct represent cross-platform OS windows that can be modified
    /// at runtime, collect OS events such as input, and swap buffers after rendering has finished.
    /// @details Instances of this struct can only be moved.
    struct window_handle {
        window_handle();
        window_handle(const window_handle& other) = delete;
        window_handle& operator=(const window_handle& other) = delete;
        window_handle(window_handle&& other) = default; // WARNING moving the window will invalidate __this ptr if captured in a callback
        window_handle& operator=(window_handle&& other) = default; // WARNING moving the window will invalidate __this ptr if captured in a callback

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

        /// @brief
        /// @param canvas_id
        void emplace_existing_emscripten(const std::string& canvas_id);

        /// @brief
        /// @return
        [[nodiscard]] static std::vector<emscripten_key_event>& get_emscripten_key_events();

        /// @brief
        /// @return
        [[nodiscard]] static std::vector<emscripten_mouse_event>& get_emscripten_mouse_events();

        /// @brief
        /// @return
        [[nodiscard]] static std::vector<emscripten_wheel_event>& get_emscripten_wheel_events();

#else

        /// @brief Creates a new window from an existing native window on native platforms.
        /// @param hwnd Raw ptr to an existing native window.
        /// @return Returns the new window.
        void emplace_existing_native(void* native_window);

        /// @brief Creates a new window from an existing SDL window.
        /// @param sdl_window Raw ptr to an existing SDL window.
        /// @return Returns the new window.
        void emplace_existing_sdl(SDL_Window* sdl_window);

        /// @brief Creates a new window on the target platform.
        void emplace_new_native();

        /// @brief Gets an opaque pointer to the OS window struct.
        /// @exception Throws a compile-time exception if the platform is web.
        [[nodiscard]] void* get_native() const;

        /// @brief Gets a raw pointer to the underlying SDL_Window.
        /// @exception Throws a compile-time exception if the platform is web.
        [[nodiscard]] SDL_Window* get_sdl() const;

        /// @brief
        /// @return
        [[nodiscard]] std::vector<SDL_Event>& get_sdl_events();

#endif

        /// @brief Gets if this instance has a value. Default created instances don't have a value
        /// until an emplace_xxx() method is called. Instances don't have a value anymore after the
        /// reset() method is called
        [[nodiscard]] bool has_value() const;

        /// @brief Resets this instance if it has a value. Default created instances don't have a
        /// value until an emplace_xxx() method is called. Instances don't have a value anymore
        /// after the reset() method is called
        void reset();

        /// @brief Sets globally if the cursor is shown for all windows.
        /// @param Whether the cursor must be shown.
        static void cursor(const bool show);

        /// @brief Sets the fullscreen mode of this instance.
        /// @param enabled Whether fullscreen mode must be enabled.
        void fullscreen(const bool enabled);

        /// @brief Gets the current display size of this instance
        [[nodiscard]] float2 get_size() const;

        /// @brief Collects all the events fired since the last call to poll() and triggers
        /// appropriate callbacks.
        void poll(const bool poll_device = true);

        /// @brief Sets a new size for this instance.
        /// @details This has no effect on Android, iOS and Emscripten platforms.
        /// @param new_size The new size in pixels.
        void resize(const float2 display_size);

        /// @brief Sets the title of this instance.
        void title(const std::string& description);

        /// @brief Sets the title of this instance.
        void update_once(const std::optional<unsigned int> max_framerate, const std::function<void(const BUNGEEGUM_USE_TIME_UNIT&)>& update_callback);

        /// @brief Sets the title of this instance.
        void update_loop(const std::optional<unsigned int> max_framerate, const std::function<void(const BUNGEEGUM_USE_TIME_UNIT&)>& update_callback);

        /// @brief
        std::vector<mouse_down_event> mouse_down_events = {};

        /// @brief
        std::vector<mouse_moved_event> mouse_moved_events = {};

        /// @brief
        std::vector<mouse_pressed_event> mouse_pressed_events = {};

        /// @brief
        std::vector<mouse_up_event> mouse_up_events = {};

        /// @brief
        std::vector<window_resized_event> window_resized_events = {};

    private:
        bool _has_value = false;
        bool _is_running = false;
        float2 _display_size = { 0, 0 };
        stopwatch _loop_watch = {};
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        std::optional<std::string> _canvas_id = std::nullopt;
#else
        std::shared_ptr<std::vector<SDL_Event>> _sdl_events = nullptr;
        SDL_Window* _sdl_window = nullptr;
#endif
    };
}
}
