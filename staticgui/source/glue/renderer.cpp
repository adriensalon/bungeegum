//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

// #if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
#define PLATFORM_WIN32 1
// #endif

// todo les autres

#include <Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>

#include <staticgui/glue/renderer.hpp>

namespace staticgui {
namespace glue {

    static Diligent::IRenderDevice* m_pDevice;
    static Diligent::IDeviceContext* m_pImmediateContext;
    static Diligent::ISwapChain* m_pSwapChain;

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
            _engine_create_info.Window = { window.get_native_handle() };

            // kidnap std::cout ? => go dans libconsole
            _factory_ptr->CreateDeviceAndSwapChainGL(_engine_create_info, &m_pDevice, &m_pImmediateContext, _swap_chain_descriptor, &m_pSwapChain);
        }
    }

    renderer::~renderer()
    {
    }

    renderer::renderer(renderer&& other)
    {
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