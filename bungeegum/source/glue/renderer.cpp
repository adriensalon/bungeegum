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

#include <ImGui/interface/ImGuiDiligentRenderer.hpp>
#include <ImGui/interface/ImGuiImplDiligent.hpp>
#include <ImGui/interface/ImGuiImplSDL.hpp>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <implot.h>

// #if RENDERER_BACKEND_DIRECTX
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
// #elif RENDERER_BACKEND_VULKAN
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>
// #elif RENDERER_BACKEND_OPENGL
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
// #endif
#include <Common/interface/RefCntAutoPtr.hpp>

#include <bungeegum/glue/renderer.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        struct diligent_renderer {
            diligent_renderer() { }
            diligent_renderer(const diligent_renderer& other)
            {
                (void)other;
            }
            diligent_renderer& operator=(const diligent_renderer& other)
            {
                (void)other;
                return *this;
            }
            diligent_renderer(diligent_renderer&& other) noexcept
            {
                (void)other;
            }
            diligent_renderer& operator=(diligent_renderer&& other)
            {
                (void)other;
            }

            Diligent::RefCntAutoPtr<Diligent::IRenderDevice> render_device;
            Diligent::RefCntAutoPtr<Diligent::IDeviceContext> device_context;
            Diligent::RefCntAutoPtr<Diligent::ISwapChain> swap_chain;
            std::unique_ptr<Diligent::ImGuiImplSDL> imgui_renderer = nullptr;
            SDL_Window* sdl_window = nullptr;
        };

        diligent_renderer& get(std::any& untyped)
        {
            return std::any_cast<diligent_renderer&>(untyped);
        }
    }

    renderer::renderer(const window& existing_window)
    {
        _diligent_renderer = std::make_any<diligent_renderer>();
        get(_diligent_renderer).sdl_window = existing_window.get_sdl_window();
        Diligent::SwapChainDesc _swap_chain_descriptor;
        if constexpr (is_renderer_backend_directx) {
            Diligent::IEngineFactoryD3D11* pFactoryD3D11 = Diligent::GetEngineFactoryD3D11();
            Diligent::EngineD3D11CreateInfo _engine_create_info;
            // _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
            pFactoryD3D11->CreateDeviceAndContextsD3D11(_engine_create_info, &(get(_diligent_renderer).render_device), &(get(_diligent_renderer).device_context));
            Diligent::Win32NativeWindow _win32_native_window(existing_window.get_native_window());
            pFactoryD3D11->CreateSwapChainD3D11(get(_diligent_renderer).render_device, get(_diligent_renderer).device_context, _swap_chain_descriptor, Diligent::FullScreenModeDesc {}, _win32_native_window, &(get(_diligent_renderer).swap_chain));
        } else if constexpr (is_renderer_backend_vulkan) {
        } else if constexpr (is_renderer_backend_opengl) {
            Diligent::IEngineFactoryOpenGL* _factory_ptr = Diligent::GetEngineFactoryOpenGL();
            Diligent::EngineGLCreateInfo _engine_create_info;
            _engine_create_info.Window = { existing_window.get_native_window() };
            // kidnap std::cout ? => go dans libconsole
            _factory_ptr->CreateDeviceAndSwapChainGL(_engine_create_info, &(get(_diligent_renderer).render_device), &(get(_diligent_renderer).device_context), _swap_chain_descriptor, &(get(_diligent_renderer).swap_chain));
        }

        const auto& SCDesc = get(_diligent_renderer).swap_chain->GetDesc();
        get(_diligent_renderer).imgui_renderer = std::make_unique<Diligent::ImGuiImplSDL>(existing_window.get_sdl_window(), get(_diligent_renderer).render_device, SCDesc.ColorBufferFormat, SCDesc.DepthBufferFormat);
        ImPlot::CreateContext();
    }

    renderer::renderer(renderer&& other)
    {
        (void)other;
        // TODO !!!!!
    }

    void renderer::rebuild_fonts()
    {
        get(_diligent_renderer).imgui_renderer->CreateDeviceObjects();
    }

    void renderer::set_clear_color(const float4 color)
    {
        _clear_color = color;
    }

    void renderer::new_frame()
    {
        auto* _rtv_ptr = get(_diligent_renderer).swap_chain->GetCurrentBackBufferRTV();
        auto* _dsv_ptr = get(_diligent_renderer).swap_chain->GetDepthBufferDSV();
        get(_diligent_renderer).device_context->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        get(_diligent_renderer).device_context->ClearRenderTarget(_rtv_ptr, std::array<float, 4> { _clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w }.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        get(_diligent_renderer).device_context->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        const auto& _swap_chain_desc = get(_diligent_renderer).swap_chain->GetDesc();
        get(_diligent_renderer).imgui_renderer->NewFrame(_swap_chain_desc.Width, _swap_chain_desc.Height, _swap_chain_desc.PreTransform);
    }

    void renderer::present()
    {
        get(_diligent_renderer).imgui_renderer->Render(get(_diligent_renderer).device_context);
        get(_diligent_renderer).swap_chain->Present();
    }

    void renderer::process_input(const std::any& input)
    {
        // window resize devrait etre manage par un diligent SDLNativeWindow
        SDL_Event _event = std::any_cast<SDL_Event>(input);
        const SDL_Event* _event_ptr = &_event;
        if (_event_ptr->type == SDL_WINDOWEVENT) {
            if (_event_ptr->window.event == SDL_WINDOWEVENT_RESIZED) {
                int _w, _h;
                SDL_GetWindowSize(get(_diligent_renderer).sdl_window, &_w, &_h);
                get(_diligent_renderer).swap_chain->Resize(_w, _h);
            }
        }
        get(_diligent_renderer).imgui_renderer->ProcessEvent(_event_ptr);
    }

}
}