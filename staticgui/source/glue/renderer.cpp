//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#define PLATFORM_EMSCRIPTEN
#elif TOOLCHAIN_PLATFORM_WIN32
#define PLATFORM_WIN32
#elif TOOLCHAIN_PLATFORM_UWP
#define PLATFORM_UNIVERSAL_WINDOWS
#elif TOOLCHAIN_PLATFORM_ANDROID
#define PLATFORM_ANDROID
#elif TOOLCHAIN_PLATFORM_LINUX
#define PLATFORM_LINUX
#elif TOOLCHAIN_PLATFORM_MACOS
#define PLATFORM_MACOS
#elif TOOLCHAIN_PLATFORM_IOS
#define PLATFORM_IOS
#endif

#include <staticgui/glue/renderer.hpp>

// #if RENDERER_BACKEND_DIRECTX
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
// #elif RENDERER_BACKEND_VULKAN
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>
// #elif RENDERER_BACKEND_OPENGL
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
// #endif
#include <Common/interface/RefCntAutoPtr.hpp>

#include <SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>

#include <ImGui/interface/ImGuiDiligentRenderer.hpp>
#include <ImGui/interface/ImGuiImplDiligent.hpp>
#include <ImGui/interface/ImGuiImplSDL.hpp>

namespace staticgui {
namespace glue {

    static std::unique_ptr<Diligent::Win32NativeWindow> Window;
    static Diligent::IRenderDevice* _render_device_impl;
    static Diligent::IDeviceContext* _device_context_impl;
    static Diligent::ISwapChain* _swap_chain_impl;
    static Diligent::ImGuiImplSDL* _imgui_renderer_impl;
    static SDL_Window* _sdl_window;

    Diligent::IRenderDevice* get_render_device(std::any& untyped)
    {
        return std::any_cast<Diligent::IRenderDevice*>(untyped);
    }

    namespace detail {
        // conversions comme window
    }

    renderer::renderer(const window& existing_window)
    {
        _sdl_window = existing_window.get_sdl_window();
        Diligent::SwapChainDesc _swap_chain_descriptor;
        if constexpr (is_renderer_backend_directx) {
            Diligent::IEngineFactoryD3D11* pFactoryD3D11 = Diligent::GetEngineFactoryD3D11();
            Diligent::EngineD3D11CreateInfo _engine_create_info;
            _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
            pFactoryD3D11->CreateDeviceAndContextsD3D11(_engine_create_info, &_render_device_impl, &_device_context_impl);
            Window = std::make_unique<Diligent::Win32NativeWindow>(existing_window.get_native_window());

            pFactoryD3D11->CreateSwapChainD3D11(_render_device_impl, _device_context_impl, _swap_chain_descriptor, Diligent::FullScreenModeDesc {}, *(Window.get()), &_swap_chain_impl);
        } else if constexpr (is_renderer_backend_vulkan) {
        } else if constexpr (is_renderer_backend_opengl) {
            Diligent::IEngineFactoryOpenGL* _factory_ptr = Diligent::GetEngineFactoryOpenGL();
            Diligent::EngineGLCreateInfo _engine_create_info;
            _engine_create_info.Window = { existing_window.get_native_window() };
            // kidnap std::cout ? => go dans libconsole
            _factory_ptr->CreateDeviceAndSwapChainGL(_engine_create_info, &_render_device_impl, &_device_context_impl, _swap_chain_descriptor, &_swap_chain_impl);
        }

        const auto& SCDesc = _swap_chain_impl->GetDesc();
        _imgui_renderer_impl = (new Diligent::ImGuiImplSDL(existing_window.get_sdl_window(), _render_device_impl, SCDesc.ColorBufferFormat, SCDesc.DepthBufferFormat));
        // bool _success = ImGui_ImplSDL2_InitForMetal(existing_window.get_sdl_window());
    }

    renderer::~renderer()
    {
    }

    renderer::renderer(renderer&& other)
    {
    }

    void renderer::set_clear_color(const std::array<float, 4>& color)
    {
        _clear_color = color;
    }

    void renderer::new_frame()
    {
        auto* _rtv_ptr = _swap_chain_impl->GetCurrentBackBufferRTV();
        auto* _dsv_ptr = _swap_chain_impl->GetDepthBufferDSV();
        _device_context_impl->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _device_context_impl->ClearRenderTarget(_rtv_ptr, _clear_color.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _device_context_impl->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        const auto& _swap_chain_desc = _swap_chain_impl->GetDesc();
        _imgui_renderer_impl->NewFrame(_swap_chain_desc.Width, _swap_chain_desc.Height, _swap_chain_desc.PreTransform);
    }

    void renderer::present()
    {
        _imgui_renderer_impl->Render(_device_context_impl);
        _swap_chain_impl->Present();
    }

    void renderer::process_input(const std::any& input)
    {
        const SDL_Event* _event = &(std::any_cast<SDL_Event>(input));
        if (_event->type == SDL_WINDOWEVENT) {
            if (_event->window.event == SDL_WINDOWEVENT_RESIZED) {
                int _w, _h;
                std::cout << "RESIZED \n";
                SDL_GetWindowSize(_sdl_window, &_w, &_h);
                _swap_chain_impl->Resize(_w, _h);
            }
        }
        _imgui_renderer_impl->ProcessEvent(&(std::any_cast<SDL_Event>(input)));
        // ImGui_ImplSDL2_ProcessEvent(&(std::any_cast<SDL_Event>(input)));
    }

}
}