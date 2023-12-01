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

// #if TOOLCHAIN_COMPILER_CLANG
// #pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
// #endif
#include <Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#endif
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID)
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>
#endif
#include <Imgui/interface/ImGuiDiligentRenderer.hpp>
#include <Imgui/interface/ImGuiImplDiligent.hpp>
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <ImGui/interface/ImGuiImplEmscripten.hpp>
#else
#include <Imgui/interface/ImGuiImplSDL.hpp>
#endif
// #if TOOLCHAIN_COMPILER_CLANG
// #pragma clang diagnostic pop
// #endif

#include <bungeegum/glue/backtrace.fwd>
#include <bungeegum/glue/renderer.fwd>



namespace bungeegum {
namespace detail {
	
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        using diligent_imgui_renderer = Diligent::ImGuiImplEmscripten;
        using diligent_native_window = Diligent::EmscriptenNativeWindow;
#else
        using diligent_imgui_renderer = Diligent::ImGuiImplSDL;
        using diligent_native_window = Diligent::NativeWindow>;
#endif

    struct renderer::renderer_data {

        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> render_device = {};
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> device_context = {};
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> swap_chain = {};
        std::shared_ptr<diligent_imgui_renderer> imgui_renderer = nullptr;
        Diligent::ImGuiImplSDL* imgui_renderer2 = nullptr;
        window* existing_window = nullptr;
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        SDL_Window* sdl_window = nullptr;
#endif
    };

    //     renderer::renderer(const window& existing_window)
    //     {
    //         // _data = std::make_shared<renderer_data>();
    //         // _data->sdl_window = existing_window.get_sdl();
    //         // Diligent::SwapChainDesc _swap_chain_descriptor;
    // #if BUNGEEGUM_USE_RENDERER_VULKAN
    //         Diligent::IEngineFactoryVk* _factory_ptr = Diligent::GetEngineFactoryVk();
    //         Diligent::EngineVkCreateInfo _engine_create_info;
    //         // _engine_create_info. TODO
    //         // _factory_ptr->
    // #elif BUNGEEGUM_USE_RENDERER_OPENGL
    //         Diligent::IEngineFactoryOpenGL* _factory_ptr = Diligent::GetEngineFactoryOpenGL();
    //         Diligent::EngineGLCreateInfo _engine_create_info;
    //         _engine_create_info.Window = { existing_window.get_native() };
    //         // kidnap std::cout ? => go dans libconsole ?
    //         _factory_ptr->CreateDeviceAndSwapChainGL(_engine_create_info, &(_data->render_device), &(_data->device_context), _swap_chain_descriptor, &(_data->swap_chain));
    // #endif
    //         const auto& SCDesc = _data->swap_chain->GetDesc();
    //         _data->imgui_renderer = std::make_shared<Diligent::ImGuiImplSDL>(existing_window.get_sdl(), _data->render_device, SCDesc.ColorBufferFormat, SCDesc.DepthBufferFormat);
    //         ImPlot::CreateContext();
    //     }

    void setup_implot_if_needed()
    {
        static ImPlotContext* _context = nullptr;
        if (!_context) {
            _context = ImPlot::CreateContext();
        }
    }

    void renderer::_consolidate(const Diligent::SwapChainDesc& swapchain_descriptor)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _data->imgui_renderer = std::make_shared<Diligent::ImGuiImplEmscripten>(
            _data->render_device,
            swapchain_descriptor.ColorBufferFormat,
            swapchain_descriptor.DepthBufferFormat);
#else
        _data->imgui_renderer = std::make_shared<Diligent::ImGuiImplSDL>(
            _data->sdl_window,
            _data->render_device,
            swapchain_descriptor.ColorBufferFormat,
            swapchain_descriptor.DepthBufferFormat);
#endif
        _data->existing_window->on_resized([&](const window_resized_event& _event) {
            float2 _rounded = math::round(_event.new_size);
            _data->swap_chain->Resize(
                static_cast<unsigned int>(_rounded.x),
                static_cast<unsigned int>(_rounded.y),
                Diligent::SURFACE_TRANSFORM_OPTIMAL);
        });
        _data->existing_window->on_sdl_event([&](const SDL_Event* _event) {
            _data->imgui_renderer->ProcessEvent(_event);
        });
    }

#if ((TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP) && BUNGEEGUM_USE_DIRECTX)
    void renderer::create_directx11(window& existing_window)
    {
        _data = std::make_shared<renderer_data>();
        _data->existing_window = &existing_window;
        _data->sdl_window = _data->existing_window->get_sdl();
        Diligent::SwapChainDesc _swap_chain_descriptor;
        Diligent::Win32NativeWindow _win32_native_window(_data->existing_window->get_native());
        Diligent::IEngineFactoryD3D11* _factory_ptr = Diligent::GetEngineFactoryD3D11();
        Diligent::EngineD3D11CreateInfo _engine_create_info;
#if defined(__DEBUG__)
        _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
#endif
        _factory_ptr->CreateDeviceAndContextsD3D11(
            _engine_create_info,
            &(_data->render_device),
            &(_data->device_context));
        _factory_ptr->CreateSwapChainD3D11(
            _data->render_device,
            _data->device_context,
            _swap_chain_descriptor,
            Diligent::FullScreenModeDesc {},
            _win32_native_window,
            &(_data->swap_chain));
        _consolidate(_swap_chain_descriptor);
        setup_implot_if_needed();
    }

    void renderer::create_directx12(window& existing_window)
    {
        _data = std::make_shared<renderer_data>();
        _data->existing_window = &existing_window;
        _data->sdl_window = _data->existing_window->get_sdl();
        Diligent::SwapChainDesc _swap_chain_descriptor;
        Diligent::Win32NativeWindow _win32_native_window(_data->existing_window->get_native());

        // todo

        _consolidate(_swap_chain_descriptor);
        setup_implot_if_needed();
    }

    // void renderer::attach_directx11(void*, void*)
    // {
    // }

    // void renderer::attach_directx12(void*, void*)
    // {
    // }
#endif

#if BUNGEEGUM_USE_OPENGL
    void renderer::create_opengl(window& existing_window)
    {
        _data = std::make_shared<renderer_data>();
        _data->existing_window = &existing_window;
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _data->sdl_window = _data->existing_window->get_sdl();
#endif
        Diligent::SwapChainDesc _swap_chain_descriptor;
        Diligent::IEngineFactoryOpenGL* _factory = Diligent::GetEngineFactoryOpenGL();
        Diligent::EngineGLCreateInfo _engine_create_info;
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _engine_create_info.Window = Diligent::EmscriptenNativeWindow("#canvas");
#else
        _engine_create_info.Window = Diligent::NativeWindow(existing_window.get_native());
#endif
        _factory->CreateDeviceAndSwapChainGL(
            _engine_create_info,
            &(_data->render_device),
            &(_data->device_context),
            _swap_chain_descriptor,
            &(_data->swap_chain));
        _consolidate(_swap_chain_descriptor);
        setup_implot_if_needed();
    }

    // void renderer::attach_opengl()
    // {
    // }
#endif

#if BUNGEEGUM_USE_VULKAN
    // void renderer::create_vulkan(window& existing)
    // {
    // }
#endif

    void renderer::rebuild_fonts()
    {
        _data->imgui_renderer->CreateDeviceObjects();
    }

    window& renderer::get_window()
    {
        return *(_data->existing_window);
    }

    const window& renderer::get_window() const
    {
        return *(_data->existing_window);
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
}
}