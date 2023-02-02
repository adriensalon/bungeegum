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

#include "windows.h"
#include <ImGui/interface/ImGuiImplWin32.hpp>

namespace staticgui {
namespace glue {

    static Diligent::IRenderDevice* m_pDevice;
    static Diligent::IDeviceContext* m_pImmediateContext;
    static Diligent::ISwapChain* m_pSwapChain;
    static Diligent::ImGuiImplDiligent* m_pImGui;

    namespace detail {
        // conversions comme window
    }

    renderer::renderer(const window& existing_window)
    {
        Diligent::SwapChainDesc _swap_chain_descriptor;
        if constexpr (is_renderer_backend_directx) {
            Diligent::IEngineFactoryD3D11* pFactoryD3D11 = Diligent::GetEngineFactoryD3D11();
            Diligent::EngineD3D11CreateInfo _engine_create_info;
            // _engine_create_info.
            _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
            pFactoryD3D11->CreateDeviceAndContextsD3D11(_engine_create_info, &m_pDevice, &m_pImmediateContext);
            Diligent::Win32NativeWindow Window { existing_window.get_native_window() };
            pFactoryD3D11->CreateSwapChainD3D11(m_pDevice, m_pImmediateContext, _swap_chain_descriptor, Diligent::FullScreenModeDesc {}, Window, &m_pSwapChain);

        } else if constexpr (is_renderer_backend_vulkan) {

        } else if constexpr (is_renderer_backend_opengl) {
            Diligent::IEngineFactoryOpenGL* _factory_ptr = Diligent::GetEngineFactoryOpenGL();
            Diligent::EngineGLCreateInfo _engine_create_info;
            _engine_create_info.Window = { existing_window.get_native_window() };
            // kidnap std::cout ? => go dans libconsole
            _factory_ptr->CreateDeviceAndSwapChainGL(_engine_create_info, &m_pDevice, &m_pImmediateContext, _swap_chain_descriptor, &m_pSwapChain);
        }

        const auto& SCDesc = m_pSwapChain->GetDesc();
        m_pImGui = (new Diligent::ImGuiImplWin32((HWND)(existing_window.get_native_window()), m_pDevice, SCDesc.ColorBufferFormat, SCDesc.DepthBufferFormat));
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
        auto* _rtv_ptr = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* _dsv_ptr = m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearRenderTarget(_rtv_ptr, _clear_color.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        const auto& _swap_chain_desc = m_pSwapChain->GetDesc();

        //
        //
        //
        //
        //
        //
        //
        //
        //
        //
        //

        // ImGui_ImplSDL2_Data* bd = ImGui_ImplSDL2_GetBackendData();
        // IM_ASSERT(bd != NULL && "Did you call ImGui_ImplSDL2_Init()?");
        // ImGuiIO& io = ImGui::GetIO();

        // // Setup display size (every frame to accommodate for window resizing)
        // int w, h;
        // int display_w, display_h;
        // SDL_GetWindowSize(bd->Window, &w, &h);
        // if (SDL_GetWindowFlags(bd->Window) & SDL_WINDOW_MINIMIZED)
        //     w = h = 0;
        // if (bd->Renderer != NULL)
        //     SDL_GetRendererOutputSize(bd->Renderer, &display_w, &display_h);
        // else
        //     SDL_GL_GetDrawableSize(bd->Window, &display_w, &display_h);
        // io.DisplaySize = ImVec2((float)w, (float)h);
        // if (w > 0 && h > 0)
        //     io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

        // // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
        // static Uint64 frequency = SDL_GetPerformanceFrequency();
        // Uint64 current_time = SDL_GetPerformanceCounter();
        // io.DeltaTime = bd->Time > 0 ? (float)((double)(current_time - bd->Time) / frequency) : (float)(1.0f / 60.0f);
        // bd->Time = current_time;

        // if (bd->PendingMouseLeaveFrame && bd->PendingMouseLeaveFrame >= ImGui::GetFrameCount() && bd->MouseButtonsDown == 0) {
        //     bd->MouseWindowID = 0;
        //     bd->PendingMouseLeaveFrame = 0;
        //     io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        // }

        // ImGui_ImplSDL2_UpdateMouseData();
        // ImGui_ImplSDL2_UpdateMouseCursor();

        // // Update game controllers (if enabled and available)
        // ImGui_ImplSDL2_UpdateGamepads();

        //
        //
        //
        //
        //
        //
        //
        //
        //
        //
        //

        m_pImGui->NewFrame(_swap_chain_desc.Width, _swap_chain_desc.Height, _swap_chain_desc.PreTransform);
    }

    void renderer::present()
    {
        m_pImGui->Render(m_pImmediateContext);
        m_pSwapChain->Present();
    }

    void renderer::process_input(const std::any& input)
    {
        ImGui_ImplSDL2_ProcessEvent(&(std::any_cast<SDL_Event>(input)));
    }

}
}