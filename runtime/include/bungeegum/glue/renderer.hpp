#pragma once

#include <memory>

#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/simd.hpp>
#include <bungeegum/glue/toolchain.hpp>
#include <bungeegum/glue/window.hpp>

/// @brief Defines if the preferred GPU renderer is DirectX.
/// @details Only D3D11 is implemented right now.
#if !defined(BUNGEEGUM_USE_RENDERER_DIRECTX)
#define BUNGEEGUM_USE_RENDERER_DIRECTX (BUNGEEGUM_ENABLE_DIRECTX && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP))
#endif

/// @brief Defines if the preferred GPU renderer is Vulkan.
#if !defined(BUNGEEGUM_USE_RENDERER_VULKAN)
#define BUNGEEGUM_USE_RENDERER_VULKAN (BUNGEEGUM_ENABLE_VULKAN && (TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID))
#endif

/// @brief Defines if the preferred GPU renderer is OpenGL.
#if !defined(BUNGEEGUM_USE_RENDERER_OPENGL)
#define BUNGEEGUM_USE_RENDERER_OPENGL (BUNGEEGUM_ENABLE_OPENGL && (TOOLCHAIN_PLATFORM_EMSCRIPTEN || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_IOS))
#endif

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct represent cross-platform GPU renderers that will select the
    /// most appropriate graphics API depending on the platform. Macros defined above can be
    /// modified to force usage of a specific platform.
    /// @details Instances of this struct can only be moved.
    struct renderer {
        renderer() = delete;
        renderer(const renderer& other) = delete;
        renderer& operator=(const renderer& other) = delete;
        renderer(renderer&& other) = default;
        renderer& operator=(renderer&& other) = default;

        /// @brief Creates an instance from a cross-platform window.
        renderer(const window& existing);

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