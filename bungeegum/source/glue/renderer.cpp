#pragma once

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
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <implot.h>

#include <bungeegum/glue/renderer.fwd>

namespace bungeegum {
namespace detail {

    namespace {

        struct diligent_renderer {
            diligent_renderer() = default;
            diligent_renderer(const diligent_renderer& other)
            {
                *this = other;
            }
            diligent_renderer& operator=(const diligent_renderer& other)
            {
                render_device = other.render_device;
                device_context = other.device_context;
                swap_chain = other.swap_chain;
                imgui_renderer = other.imgui_renderer;
                sdl_window = other.sdl_window;
                return *this;
            }
            diligent_renderer(diligent_renderer&& other)
            {
                *this = std::move(other);
            }
            diligent_renderer& operator=(diligent_renderer&& other)
            {
                render_device = std::move(other.render_device);
                device_context = std::move(other.device_context);
                swap_chain = std::move(other.swap_chain);
                imgui_renderer = std::move(other.imgui_renderer);
                sdl_window = std::move(other.sdl_window);
                return *this;
            }
            Diligent::RefCntAutoPtr<Diligent::IRenderDevice> render_device = {};
            Diligent::RefCntAutoPtr<Diligent::IDeviceContext> device_context = {};
            Diligent::RefCntAutoPtr<Diligent::ISwapChain> swap_chain = {};
            std::shared_ptr<Diligent::ImGuiImplSDL> imgui_renderer = nullptr;
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
        diligent_renderer& _renderer = get(_diligent_renderer);
        _renderer.sdl_window = existing_window.get_sdl_window();
        Diligent::SwapChainDesc _swap_chain_descriptor;
        if constexpr (is_renderer_backend_directx) {
            Diligent::IEngineFactoryD3D11* pFactoryD3D11 = Diligent::GetEngineFactoryD3D11();
            Diligent::EngineD3D11CreateInfo _engine_create_info;
            // _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
            pFactoryD3D11->CreateDeviceAndContextsD3D11(_engine_create_info, &(_renderer.render_device), &(_renderer.device_context));
            Diligent::Win32NativeWindow _win32_native_window(existing_window.get_native_window());
            pFactoryD3D11->CreateSwapChainD3D11(_renderer.render_device, _renderer.device_context, _swap_chain_descriptor, Diligent::FullScreenModeDesc {}, _win32_native_window, &(_renderer.swap_chain));
        } else if constexpr (is_renderer_backend_vulkan) {
        } else if constexpr (is_renderer_backend_opengl) {
            Diligent::IEngineFactoryOpenGL* _factory_ptr = Diligent::GetEngineFactoryOpenGL();
            Diligent::EngineGLCreateInfo _engine_create_info;
            _engine_create_info.Window = { existing_window.get_native_window() };
            // kidnap std::cout ? => go dans libconsole ?
            _factory_ptr->CreateDeviceAndSwapChainGL(_engine_create_info, &(_renderer.render_device), &(_renderer.device_context), _swap_chain_descriptor, &(_renderer.swap_chain));
        }

        const auto& SCDesc = _renderer.swap_chain->GetDesc();
        _renderer.imgui_renderer = std::make_shared<Diligent::ImGuiImplSDL>(existing_window.get_sdl_window(), _renderer.render_device, SCDesc.ColorBufferFormat, SCDesc.DepthBufferFormat);
        ImPlot::CreateContext();
    }

    void renderer::rebuild_fonts()
    {
        diligent_renderer& _renderer = get(_diligent_renderer);
        _renderer.imgui_renderer->CreateDeviceObjects();
    }

    void renderer::set_clear_color(const float4 color)
    {
        _clear_color = color;
    }

    void renderer::new_frame()
    {
        diligent_renderer& _renderer = get(_diligent_renderer);
        Diligent::ITextureView* _rtv_ptr = _renderer.swap_chain->GetCurrentBackBufferRTV();
        Diligent::ITextureView* _dsv_ptr = _renderer.swap_chain->GetDepthBufferDSV();
        _renderer.device_context->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        // bizarre un peu la conversion
        _renderer.device_context->ClearRenderTarget(_rtv_ptr, std::array<float, 4> { _clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w }.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _renderer.device_context->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        const Diligent::SwapChainDesc& _swap_chain_desc = _renderer.swap_chain->GetDesc();
        _renderer.imgui_renderer->NewFrame(_swap_chain_desc.Width, _swap_chain_desc.Height, _swap_chain_desc.PreTransform);
    }

    void renderer::present()
    {
        diligent_renderer& _renderer = get(_diligent_renderer);
        _renderer.imgui_renderer->Render(_renderer.device_context);
        _renderer.swap_chain->Present();
    }

    void renderer::process_input(const std::any& input)
    {
        diligent_renderer& _renderer = get(_diligent_renderer);
        SDL_Event _event = std::any_cast<SDL_Event>(input);
        if (_event.type == SDL_WINDOWEVENT) {
            if (_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                int _width, _height;
                SDL_GetWindowSize(_renderer.sdl_window, &_width, &_height);
                _renderer.swap_chain->Resize(_width, _height);
            }
        }
        const SDL_Event* _event_ptr = &_event;
        _renderer.imgui_renderer->ProcessEvent(_event_ptr);
    }

    void renderer::resize(const uint2 size)
    {
        diligent_renderer& _renderer = get(_diligent_renderer);
        _renderer.swap_chain->Resize(static_cast<int>(size.x), static_cast<int>(size.y));
    }
}
}