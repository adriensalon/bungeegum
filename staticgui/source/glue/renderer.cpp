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
    }

    void renderer::debug_fn()
    {
        // auto* pCtx = pContext; //reinterpret_cast<Diligent::IDeviceContext*>(context);
        auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
        auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
        m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Clear the back buffer
        static float a = 0.150f;
        a += 0.01f;
        const float ClearColor[] = { std::fmin(1.f, a), 0.350f, 0.850f, 1.0f };
        // Let the engine perform required state transitions
        m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        m_pImmediateContext->ClearDepthStencil(pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const auto& SCDesc = m_pSwapChain->GetDesc();
        m_pImGui->NewFrame(SCDesc.Width, SCDesc.Height, SCDesc.PreTransform);

        bool aa = true;

        ImGui::ShowDemoWindow(&aa);

        ImGui::Begin("my_window");

        ImGui::End();

        m_pImGui->Render(m_pImmediateContext);

        m_pSwapChain->Present();
    }

    renderer::~renderer()
    {
    }

    renderer::renderer(renderer&& other)
    {
    }

    void renderer::process_event(const std::any& event)
    {
        ImGui_ImplSDL2_ProcessEvent(&(std::any_cast<SDL_Event>(event)));
    }

    renderer& renderer::operator=(renderer&& other)
    {
        return *this;
    }

    void* renderer::get_native_device() const
    {
        return nullptr;
    }

    void* renderer::get_native_swap_chain() const
    {
        return nullptr;
    }

    void* renderer::get_native_context() const
    {
        return nullptr;
    }

}
}