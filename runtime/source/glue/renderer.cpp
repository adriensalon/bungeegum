// #define PLATFORM_WIN32
#include <bungeegum/glue/backtrace.fwd>
#include <bungeegum/glue/renderer.fwd> // TOOLCHAIN HERE !

#include <array>

#include <imgui.h>
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
// imgui emscripten ?
#else
#include <SDL_events.h>
#include <imgui_impl_sdl2.h>
#endif
#include <implot.h>

// define those before including diligent headers
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN && !defined(PLATFORM_EMSCRIPTEN)
#define PLATFORM_EMSCRIPTEN
#elif TOOLCHAIN_PLATFORM_WIN32 && !defined(PLATFORM_WIN32)
#define PLATFORM_WIN32
#elif TOOLCHAIN_PLATFORM_UWP && !defined(PLATFORM_UNIVERSAL_WINDOWS)
#define PLATFORM_UNIVERSAL_WINDOWS
#elif TOOLCHAIN_PLATFORM_ANDROID && !defined(PLATFORM_ANDROID)
#define PLATFORM_ANDROID
#elif TOOLCHAIN_PLATFORM_LINUX && !defined(PLATFORM_LINUX)
#define PLATFORM_LINUX
#elif TOOLCHAIN_PLATFORM_MACOS && !defined(PLATFORM_MACOS)
#define PLATFORM_MACOS
#elif TOOLCHAIN_PLATFORM_IOS && !defined(PLATFORM_IOS)
#define PLATFORM_IOS
#endif

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
#include <Imgui/interface/ImGuiImplEmscripten.hpp>
#else
#include <Imgui/interface/ImGuiImplSDL.hpp>
#endif


namespace bungeegum {
namespace detail {

    struct renderer::renderer_data {
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> render_device = {};
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> device_context = {};
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> swap_chain = {};
        window* existing_window = nullptr;
		
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        std::shared_ptr<Diligent::ImGuiImplEmscripten> imgui_renderer = nullptr;
#else
        std::shared_ptr<Diligent::ImGuiImplSDL> imgui_renderer = nullptr;
        SDL_Window* sdl_window = nullptr;
#endif
    };
	
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
    }

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
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
#endif

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
		_engine_create_info.GraphicsAPIVersion = Diligent::Version(3, 1);
		
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

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID)
    void renderer::create_vulkan(window& existing)
    {
		(void)existing;
    }
#endif

    void renderer::rebuild_fonts()
    {
        _data->imgui_renderer->CreateDeviceObjects();
    }
	
	bool renderer::has_renderer() const
	{
		return _data.operator bool();
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
	
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
	void renderer::consume_emscripten_key_events(std::vector<emscripten_key_event>& events)
	{
		for (const emscripten_key_event& _event : events) {
			_data->imgui_renderer->OnKeyEvent(_event.event_type, _event.event);
		}
		events.clear();
	}

	void renderer::consume_emscripten_mouse_events(std::vector<emscripten_mouse_event>& events)
	{
		for (const emscripten_mouse_event& _event : events) {
			_data->imgui_renderer->OnMouseEvent(_event.event_type, _event.event);
		}
		events.clear();
	}

	void renderer::consume_emscripten_wheel_events(std::vector<emscripten_wheel_event>& events)
	{
		for (const emscripten_wheel_event& _event : events) {
			_data->imgui_renderer->OnWheelEvent(_event.event_type, _event.event);
		}
		events.clear();
	}
#else		
	void renderer::consume_sdl_events(std::vector<SDL_Event>& events)
	{
		for (const SDL_Event& _event : events) {
			_data->imgui_renderer->OnEvent(_event);
		}
		events.clear();
	}
#endif

    void renderer::present()
    {
        if (!_is_rendering) {
            throw backtraced_exception("TODO");
        }
        _data->imgui_renderer->Render(_data->device_context);
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _data->swap_chain->Present();
#endif
        _is_rendering = false;
    }	

	void renderer::resize(const float2 display_size)
	{
		float2 _rounded = math::round(display_size);
		_data->swap_chain->Resize(
			static_cast<unsigned int>(_rounded.x),
			static_cast<unsigned int>(_rounded.y),
			Diligent::SURFACE_TRANSFORM_OPTIMAL);
	}
}
}