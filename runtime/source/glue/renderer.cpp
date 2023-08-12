#pragma once

#include <array>

#include <imgui.h>
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
// imgui emscripten ?
#else
#include <imgui_impl_sdl2.h>
#endif
#include <implot.h>

// define those before including diligent headers
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

#include <Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>
#include <ImGui/interface/ImGuiDiligentRenderer.hpp>
#include <ImGui/interface/ImGuiImplDiligent.hpp>
#include <ImGui/interface/ImGuiImplSDL.hpp>

#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/renderer.hpp>

namespace bungeegum {
namespace detail {

    struct renderer::renderer_data {
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> render_device = {};
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> device_context = {};
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> swap_chain = {};
        std::shared_ptr<Diligent::ImGuiImplSDL> imgui_renderer = nullptr;
        SDL_Window* sdl_window = nullptr;
    };

    //     renderer::renderer(const window& existing_window)
    //     {
    //         // _data = std::make_shared<renderer_data>();
    //         // _data->sdl_window = existing_window.get_sdl_window();
    //         // Diligent::SwapChainDesc _swap_chain_descriptor;
    // #if BUNGEEGUM_USE_RENDERER_VULKAN
    //         Diligent::IEngineFactoryVk* _factory_ptr = Diligent::GetEngineFactoryVk();
    //         Diligent::EngineVkCreateInfo _engine_create_info;
    //         // _engine_create_info. TODO
    //         // _factory_ptr->
    // #elif BUNGEEGUM_USE_RENDERER_OPENGL
    //         Diligent::IEngineFactoryOpenGL* _factory_ptr = Diligent::GetEngineFactoryOpenGL();
    //         Diligent::EngineGLCreateInfo _engine_create_info;
    //         _engine_create_info.Window = { existing_window.get_native_window() };
    //         // kidnap std::cout ? => go dans libconsole ?
    //         _factory_ptr->CreateDeviceAndSwapChainGL(_engine_create_info, &(_data->render_device), &(_data->device_context), _swap_chain_descriptor, &(_data->swap_chain));
    // #endif
    //         const auto& SCDesc = _data->swap_chain->GetDesc();
    //         _data->imgui_renderer = std::make_shared<Diligent::ImGuiImplSDL>(existing_window.get_sdl_window(), _data->render_device, SCDesc.ColorBufferFormat, SCDesc.DepthBufferFormat);
    //         ImPlot::CreateContext();
    //     }

#if BUNGEEGUM_USE_DIRECTX
    renderer renderer::create_directx11(const window& existing)
    {
        renderer _renderer;
        _renderer._data = std::make_shared<renderer_data>();
        _renderer._data->sdl_window = existing.get_sdl_window();
        Diligent::SwapChainDesc _swap_chain_descriptor;
        Diligent::IEngineFactoryD3D11* _factory_ptr = Diligent::GetEngineFactoryD3D11();
        Diligent::EngineD3D11CreateInfo _engine_create_info;
        // _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
        _factory_ptr->CreateDeviceAndContextsD3D11(
            _engine_create_info,
            &(_renderer._data->render_device),
            &(_renderer._data->device_context));
        Diligent::Win32NativeWindow _win32_native_window(existing.get_native_window());
        _factory_ptr->CreateSwapChainD3D11(
            _renderer._data->render_device,
            _renderer._data->device_context,
            _swap_chain_descriptor,
            Diligent::FullScreenModeDesc {},
            _win32_native_window,
            &(_renderer._data->swap_chain));
        _renderer._data->imgui_renderer = std::make_shared<Diligent::ImGuiImplSDL>(
            _renderer._data->sdl_window,
            _renderer._data->render_device,
            _swap_chain_descriptor.ColorBufferFormat,
            _swap_chain_descriptor.DepthBufferFormat);
        ImPlot::CreateContext();
        return _renderer;
    }

    // renderer renderer::create_directx12(const window& existing)
    // {
    // }

    // renderer renderer::attach_directx11(void*, void*)
    // {
    // }

    // renderer renderer::attach_directx12(void*, void*)
    // {
    // }
#endif

#if BUNGEEGUM_USE_OPENGL
    // renderer renderer::create_opengl(const window& existing)
    // {
    // }

    // renderer renderer::attach_opengl()
    // {
    // }
#endif

#if BUNGEEGUM_USE_VULKAN
    // renderer renderer::create_vulkan(const window& existing)
    // {
    // }
#endif

    void renderer::rebuild_fonts()
    {
        _data->imgui_renderer->CreateDeviceObjects();
    }

    void renderer::new_frame()
    {
        if (_is_rendering) {
            throw backtraced_exception("TODO");
        }
        Diligent::ITextureView* _rtv_ptr = _data->swap_chain->GetCurrentBackBufferRTV();
        Diligent::ITextureView* _dsv_ptr = _data->swap_chain->GetDepthBufferDSV();
        _data->device_context->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        const std::array<float, 4> _clear_color_array = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
        _data->device_context->ClearRenderTarget(_rtv_ptr, _clear_color_array.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _data->device_context->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        const Diligent::SwapChainDesc& _swap_chain_desc = _data->swap_chain->GetDesc();
        _data->imgui_renderer->NewFrame(_swap_chain_desc.Width, _swap_chain_desc.Height, _swap_chain_desc.PreTransform);
        _is_rendering = true;
    }

    void renderer::present()
    {
        if (!_is_rendering) {
            throw backtraced_exception("TODO");
        }
        _data->imgui_renderer->Render(_data->device_context);
        _data->swap_chain->Present();
        _is_rendering = false;
    }

    void renderer::process_sdl_event_for_imgui(const SDL_Event* event)
    {
        _data->imgui_renderer->ProcessEvent(event);
    }

    void renderer::process_window_resized_event(const window_resized_event& event)
    {
        _data->swap_chain->Resize(static_cast<uint1>(event.new_size.x), static_cast<uint1>(event.new_size.y));
    }

    void renderer::resize_swapchain(const uint2 size)
    {
        _data->swap_chain->Resize(size.x, size.y);
    }
}
}