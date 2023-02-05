//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <array>

#include <staticgui/glue/toolchain.hpp>
#include <staticgui/glue/window.hpp>

#define RENDERER_BACKEND_DIRECTX (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
#define RENDERER_BACKEND_VULKAN (TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID)
#define RENDERER_BACKEND_OPENGL (TOOLCHAIN_PLATFORM_EMSCRIPTEN || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_IOS)

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

        void set_clear_color(const std::array<float, 4>& color);

        void new_frame();

        void present();

        void process_input(const std::any& input);

    private:
        struct diligent_renderer;
        std::unique_ptr<diligent_renderer> _renderer_impl = nullptr;
        std::array<float, 4> _clear_color = { 0.f, 0.f, 0.f, 1.f };
    };

}
}