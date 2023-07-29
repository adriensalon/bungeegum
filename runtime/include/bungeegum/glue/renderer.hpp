#pragma once

#include <memory>

#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/simd.hpp>
#include <bungeegum/glue/toolchain.hpp>
#include <bungeegum/glue/window.hpp>

/// @brief Defines if we use DirectX functionnality.
#if !defined(BUNGEEGUM_USE_DIRECTX)
#define BUNGEEGUM_USE_DIRECTX (BUNGEEGUM_ENABLE_DIRECTX && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP))
#endif

/// @brief Defines if we use Vulkan functionnality.
#if !defined(BUNGEEGUM_USE_VULKAN)
#define BUNGEEGUM_USE_VULKAN (BUNGEEGUM_ENABLE_VULKAN && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID))
#endif

/// @brief Defines if we use OpenGL functionnality.
#if !defined(BUNGEEGUM_USE_OPENGL)
#define BUNGEEGUM_USE_OPENGL (BUNGEEGUM_ENABLE_OPENGL && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID || TOOLCHAIN_PLATFORM_EMSCRIPTEN || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_IOS))
#endif

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct represent cross-platform GPU renderers that will select the
    /// most appropriate graphics API depending on the platform. Macros defined above can be
    /// modified to force usage of a specific platform.
    /// @details Instances of this struct can only be moved.
    struct renderer {
    private:
        renderer() = default;

    public:
        renderer(const renderer& other) = delete;
        renderer& operator=(const renderer& other) = delete;
        renderer(renderer&& other) = default;
        renderer& operator=(renderer&& other) = default;

#if BUNGEEGUM_USE_DIRECTX
        /// @brief Creates an instance from an existing window with the DirectX 11 API.
        static renderer create_directx11(const window& existing);

        /// @brief Creates an instance from an existing window with the DirectX 12 API.
        static renderer create_directx12(const window& existing);

        /// @brief Creates an instance from an existing DirectX 11 context.
        static renderer attach_directx11(void*, void*);

        /// @brief Creates an instance from an existing DirectX 12 context.
        static renderer attach_directx12(void*, void*);
#endif

#if BUNGEEGUM_USE_OPENGL
        /// @brief Creates an instance from an existing window with the OpenGL API.
        static renderer create_opengl(const window& existing);

        /// @brief Creates an instance from an existing OpenGL context.
        static renderer attach_opengl();
#endif

#if BUNGEEGUM_USE_VULKAN
        /// @brief Creates an instance from an existing window with the Vulkan API.
        static renderer create_vulkan(const window& existing);
#endif

        /// @brief Begins a new frame, enabling all drawing commands.
        /// @exception Throws a backtraced exception if a new frame has already begun.
        void new_frame();

        /// @brief Ends the frame, disabling all drawing commands. Swaps the window buffers.
        /// @exception Throws a backtraced exception if the frame has already been ended.
        void present();

        /// @brief Processes ImGui events from SDL. (To be removed)
        void process_sdl_event_for_imgui(const SDL_Event* event);

        /// @brief Processes window resized events from a window, and resizes the swapchain.
        void process_window_resized_event(const window_resized_event& event);

        /// @brief Rebuilds the ImGui fonts. (To be removed)
        void rebuild_fonts();

        /// @brief Resizes the swapchain.
        void resize_swapchain(const uint2 size);

        /// @brief The color to use when clearing the screen.
        float4 clear_color = { 0.f, 0.f, 0.f, 1.f };

    private:
        bool1 _is_rendering = false;

        struct renderer_data;
        std::shared_ptr<renderer_data> _data = nullptr;
    };
}
}