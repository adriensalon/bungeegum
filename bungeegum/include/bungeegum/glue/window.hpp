#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#include <bungeegum/glue/simd.hpp>
#include <bungeegum/glue/toolchain.hpp>

/// @brief Defines if we use a native window.
#if !defined(BUNGEEGUM_USE_WINDOW_NATIVE)
#define BUNGEEGUM_USE_WINDOW_NATIVE (!TOOLCHAIN_PLATFORM_EMSCRIPTEN)
#endif

/// @brief Defines if we use a web browser window.
#if !defined(BUNGEEGUM_USE_WINDOW_WEB)
#define BUNGEEGUM_USE_WINDOW_WEB (TOOLCHAIN_PLATFORM_EMSCRIPTEN)
#endif

#if BUNGEEGUM_USE_WINDOW_NATIVE
struct SDL_Window;
union SDL_Event;
#endif

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct represent cross-platform window resized event data.
    struct window_resized_event {
        bool1 is_fullscreen;
        float2 new_size;
    };

    /// @brief Instances of this struct represent cross-platform mouse moved event data.
    struct mouse_moved_event {
        float2 absolute_position;
        float2 relative_position;
    };

    /// @brief Instances of this struct represent cross-platform mouse down event data.
    struct mouse_down_event {
        uint1 button_index;
    };

    /// @brief Instances of this struct represent cross-platform mouse up event data.
    struct mouse_up_event {
        uint1 button_index;
    };

    /// @brief Instances of this struct represent cross-platform mouse pressed event data.
    struct mouse_pressed_event {
        uint1 button_index;
    };

    /// @brief Instances of this struct represent cross-platform OS windows that can be modified
    /// at runtime, collect OS events such as input, and swap buffers after rendering has finished.
    /// @details Instances of this struct can only be moved.
    struct window {
        window();
        window(const window& other) = delete;
        window& operator=(const window& other) = delete;
        window(window&& other) = default;
        window& operator=(window&& other) = default;
        ~window();

#if BUNGEEGUM_USE_WINDOW_NATIVE
        /// @brief Creates an instance from an opaque OS window.
        /// @exception Throws a compile-time exception if the platform is web.
        window(void* native_window);
#endif

#if BUNGEEGUM_USE_WINDOW_NATIVE
        /// @brief Creates an instance from an existing SDL window.
        /// @exception Throws a compile-time exception if the platform is web.
        window(SDL_Window* sdl_window);
#endif

#if BUNGEEGUM_USE_WINDOW_NATIVE
        /// @brief Gets an opaque pointer to the OS window struct.
        /// @exception Throws a compile-time exception if the platform is web.
        [[nodiscard]] void* get_native_window() const;
#endif

#if BUNGEEGUM_USE_WINDOW_NATIVE
        /// @brief Gets a raw pointer to the underlying SDL_Window.
        /// @exception Throws a compile-time exception if the platform is web.
        [[nodiscard]] SDL_Window* get_sdl_window() const;
#endif

        /// @brief Gets the current size of this instance.
        [[nodiscard]] uint2 get_size() const;

        /// @brief Sets a callback to be trigged each frame after run_loop() has been fired.
        void on_update(const std::function<void()>& update_callback);

        /// @brief Sets a callback to be triggered each time a mouse down event is received.
        /// @details Event callbacks are fired after poll() has been invoked.
        void on_mouse_down(const std::function<void(const mouse_down_event&)>& mouse_down_callback);

        /// @brief Sets a callback to be triggered each time a mouse moved event is received.
        /// @details Event callbacks are fired after poll() has been invoked.
        void on_mouse_moved(const std::function<void(const mouse_moved_event&)>& mouse_moved_callback);

        /// @brief Sets a callback to be triggered each time a mouse pressed event is received.
        /// @details Event callbacks are fired after poll() has been invoked.
        void on_mouse_pressed(const std::function<void(const mouse_pressed_event&)>& mouse_pressed_callback);

        /// @brief Sets a callback to be triggered each time a mouse up event is received.
        /// @details Event callbacks are fired after poll() has been invoked.
        void on_mouse_up(const std::function<void(const mouse_up_event&)>& mouse_up_callback);

#if BUNGEEGUM_USE_WINDOW_NATIVE
        /// @brief Sets a callback to be triggered each time a SDL_Event is received.
        /// @details Event callbacks are fired after poll() has been invoked.
        /// @exception Throws a compile-time exception if the platform is web.
        void on_sdl_event(const std::function<void(const SDL_Event*)>& sdl_event_callback);
#endif

        /// @brief Sets a callback to be triggered each time a window resized event is received.
        /// @details Event callbacks are fired after poll() has been invoked.
        void on_window_resized(const std::function<void(const window_resized_event&)>& window_resized_callback);

        /// @brief Collects all the events fired since the last call to poll() and triggers
        /// appropriate callbacks.
        bool poll();

        /// @brief Blocks the thread and starts an infinite loop that executes the callback defined
        /// with on_update() then swaps buffers.
        void run_loop();

        /// @brief Sets the fullscreen mode of this instance.
        void set_fullscreen(const bool enabled);

        /// @brief Sets the size of this instance.
        void set_size(const uint2 size);

        /// @brief Sets the title of this instance.
        void set_title(const std::string& title);

        /// @brief Sets globally if the cursor is shown.
        /// @details Applies to all windows on native platforms.
        static void show_cursor(const bool show);

    private:
        bool1 _is_running = false;
        std::function<void()> _update_callback = nullptr;
        std::function<void(const window_resized_event&)> _window_resized_callback = nullptr;
        std::function<void(const mouse_moved_event&)> _mouse_moved_callback = nullptr;
        std::function<void(const mouse_down_event&)> _mouse_down_callback = nullptr;
        std::function<void(const mouse_up_event&)> _mouse_up_callback = nullptr;
        std::function<void(const mouse_pressed_event&)> _mouse_pressed_callback = nullptr;

        struct window_data;
        std::shared_ptr<window_data> _data = nullptr;
    };

}
}
