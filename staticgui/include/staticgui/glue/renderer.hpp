//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/toolchain.hpp>
#include <staticgui/glue/window.hpp>

#define RENDERER_BACKEND_DIRECTX (staticgui::glue::is_platform_win32 || staticgui::glue::is_platform_uwp)
#define RENDERER_BACKEND_VULKAN (staticgui::glue::is_platform_linux || staticgui::glue::is_platform_android)
#define RENDERER_BACKEND_OPENGL (staticgui::glue::is_platform_emscripten || staticgui::glue::is_platform_macos || staticgui::glue::is_platform_ios)

namespace staticgui {
namespace glue {

    constexpr bool is_renderer_backend_directx = RENDERER_BACKEND_DIRECTX;

    constexpr bool is_renderer_backend_vulkan = RENDERER_BACKEND_VULKAN;

    constexpr bool is_renderer_backend_opengl = RENDERER_BACKEND_OPENGL;

    struct renderer {
        renderer(const window& existing);
        ~renderer();
        renderer(const renderer& other) = delete;
        renderer& operator=(const renderer& other) = delete;
        renderer(renderer&& other);
        renderer& operator=(renderer&& other);

        void* get_native_device() const;

        void* get_native_swap_chain() const;

        void* get_native_context() const;

    private:
        std::any _device_context_impl = nullptr;
        std::any _swap_chain_impl = nullptr;
        std::any _imgui_renderer_impl = nullptr;
    };

}
}