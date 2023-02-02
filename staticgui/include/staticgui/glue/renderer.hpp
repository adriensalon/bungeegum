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

namespace staticgui {
namespace glue {

    constexpr bool is_renderer_backend_directx = is_platform_win32 || is_platform_uwp;

    constexpr bool is_renderer_backend_vulkan = is_platform_linux || is_platform_android;

    constexpr bool is_renderer_backend_opengl = is_platform_emscripten || is_platform_macos || is_platform_ios;

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