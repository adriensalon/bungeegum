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
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>
#include <ImGui/interface/ImGuiDiligentRenderer.hpp>
#include <ImGui/interface/ImGuiImplDiligent.hpp>
#include <ImGui/interface/ImGuiImplSDL.hpp>
// #if TOOLCHAIN_COMPILER_CLANG
// #pragma clang diagnostic pop
// #endif

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

    static Diligent::RefCntAutoPtr<Diligent::IRenderDevice> _global_render_device = {};
    static Diligent::RefCntAutoPtr<Diligent::IDeviceContext> _global_device_context = {};
    static Diligent::RefCntAutoPtr<Diligent::ISwapChain> _global_swap_chain = {};

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
        _global_render_device = _renderer._data->render_device;
        _global_device_context = _renderer._data->device_context;
        _global_swap_chain = _renderer._data->swap_chain;
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














    // void font_handle::emplace(
    //     renderer& rasterizer,
    //     const std::filesystem::path& filename,
    //     const float size,
    //     const std::optional<font_config>& config,
    //     unsigned short* ranges)
    // {
    //     // if (!rasterizer.has_value()) {
    //     //     throw backtraced_exception { "[rendering] Impossible to create font because rasterizer has no value" };
    //     // }
    //     reset();
    //     (void)rasterizer;

    //     int _raw_width, _raw_height = 0;
    //     unsigned char* _raw_pixels = nullptr;
    //     // ImGui::SetCurrentContext(rasterizer._data->_imgui_context);
    //     ImGuiIO& _io = ImGui::GetIO();

    //     std::string _spath = filename.generic_string();
    //     const char* _cpath = _spath.c_str();
    //     ImFontConfig _imgui_config; 
    //     if (config.has_value()) {
    //         const font_config& _font_config = config.value();      
    //         _imgui_config.FontNo = static_cast<int>(_font_config.index);
    //         _imgui_config.OversampleH = static_cast<int>(_font_config.oversample_horizontal);
    //         _imgui_config.OversampleV = static_cast<int>(_font_config.oversample_vertical);
    //         _imgui_config.PixelSnapH = _font_config.pixel_snap_horizontal;
    //         _imgui_config.GlyphExtraSpacing = ImVec2 { _font_config.glyph_extra_spacing.x, _font_config.glyph_extra_spacing.y };
    //         _imgui_config.GlyphOffset = ImVec2 { _font_config.glyph_extra_spacing.x, _font_config.glyph_extra_spacing.y };
    //         _imgui_config.GlyphMinAdvanceX = _font_config.glyph_min_advance; // Use if you want to make the icon monospaced
    //     }
    //     _imgui_font = _io.Fonts->AddFontFromFileTTF(_cpath, size, &_imgui_config, ranges);

    //     _io.Fonts->Build();
    //     _io.Fonts->GetTexDataAsRGBA32(&_raw_pixels, &_raw_width, &_raw_height);
    //     // rasterizer._font_texture.emplace(
    //     //     rasterizer,
    //     //     std::vector<unsigned char>(_raw_pixels, _raw_pixels + (4 * _raw_width * _raw_height)),
    //     //     static_cast<std::size_t>(_raw_width),
    //     //     static_cast<std::size_t>(_raw_height));
    //     // _io.Fonts->TexID = rasterizer._font_texture.get();
    //     _has_value = true;
    // }
    
    void font_handle::emplace(
        renderer& rasterizer,
        const void* ttf,
        const std::size_t count,
        const float size,
        const std::optional<font_config>& config,
        unsigned short* ranges)
    {
        // if (!rasterizer.has_value()) {
        //     throw backtraced_exception { "[rendering] Impossible to create font because rasterizer has no value" };
        // }
        reset();
        (void)rasterizer;

        int _raw_width, _raw_height = 0;
        unsigned char* _raw_pixels = nullptr;
        // ImGui::SetCurrentContext(rasterizer._imgui_context);
        ImGuiIO& _io = ImGui::GetIO();
 
        ImFontConfig _imgui_config; 
        if (config.has_value()) {
            const font_config& _font_config = config.value();      
            _imgui_config.FontNo = static_cast<int>(_font_config.index);
            _imgui_config.OversampleH = static_cast<int>(_font_config.oversample_horizontal);
            _imgui_config.OversampleV = static_cast<int>(_font_config.oversample_vertical);
            _imgui_config.PixelSnapH = _font_config.pixel_snap_horizontal;
            _imgui_config.GlyphExtraSpacing = ImVec2 { _font_config.glyph_extra_spacing.x, _font_config.glyph_extra_spacing.y };
            _imgui_config.GlyphOffset = ImVec2 { _font_config.glyph_extra_spacing.x, _font_config.glyph_extra_spacing.y };
            _imgui_config.GlyphMinAdvanceX = _font_config.glyph_min_advance; // Use if you want to make the icon monospaced
            // static const unsigned short _ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        }
        _imgui_font = _io.Fonts->AddFontFromMemoryCompressedTTF(ttf, static_cast<int>(count), size, &_imgui_config, ranges);

        _io.Fonts->Build();
        _io.Fonts->GetTexDataAsRGBA32(&_raw_pixels, &_raw_width, &_raw_height);
        // rasterizer._font_texture.emplace(
        //     rasterizer,
        //     std::vector<unsigned char>(_raw_pixels, _raw_pixels + (4 * _raw_width * _raw_height)),
        //     static_cast<std::size_t>(_raw_width),
        //     static_cast<std::size_t>(_raw_height));
        // _io.Fonts->TexID = rasterizer._font_texture.get();
        _has_value = true;
    }

    ImFont* font_handle::get() const
    {
        if (_has_value) {
            return _imgui_font;
        }
        return nullptr;
    }

    bool font_handle::has_value() const
    {
        return _has_value;
    }

    void font_handle::reset()
    {
        if (_has_value) {
            _imgui_font = nullptr;
            _has_value = false;
        }
    }




















    // void texture_handle::emplace(
    //     renderer& rasterizer,
    //     const std::filesystem::path filename)
    // {
    //     // if (!rasterizer.has_value()) {
    //     //     throw backtraced_exception { "[rendering exception] impossible to create texture because rasterizer has no value" };
    //     // }
    //     reset();

    //     std::string _spath = filename.generic_string();
    //     const char* _cpath = _spath.c_str();
    //     int _width, _height, _channels;
    //     unsigned char* _data = stbi_load(_cpath, &_width, &_height, &_channels, 4);
    //     if (_channels < 3 || _channels > 4) {
    //         throw backtraced_exception("invalid channels");
    //     }
    //     std::vector<unsigned char> _pixels(_data, _data + 4 * _width * _height);
    //     stbi_image_free(_data);

    //     Diligent::TextureDesc _texture_desc;
    //     _texture_desc.Name = "uiw user texture";
    //     _texture_desc.Type = Diligent::RESOURCE_DIM_TEX_2D;
    //     _texture_desc.Width = static_cast<Diligent::Uint32>(_width);
    //     _texture_desc.Height = static_cast<Diligent::Uint32>(_height);
    //     _texture_desc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM;
    //     _texture_desc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
    //     _texture_desc.Usage = Diligent::USAGE_IMMUTABLE;
    //     Diligent::TextureSubResData _texture_subres_data[] = { { _pixels.data(), 4 * Diligent::Uint64 { _texture_desc.Width } } };
    //     Diligent::TextureData _texture_data(_texture_subres_data, _countof(_texture_subres_data));
    //     rasterizer._data->_render_device->CreateTexture(_texture_desc, &_texture_data, &_diligent_texture);
    //     _diligent_texture_view = _diligent_texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
    //     _has_value = true;
    // }

    void texture_handle::emplace(
        const std::vector<unsigned char>& pixels,
        const std::size_t width,
        const std::size_t height)
    {
        // if (!rasterizer.has_value()) {
        //     throw backtraced_exception { "[rendering exception] impossible to create texture because rasterizer has no value" };
        // }
        reset();
        Diligent::TextureDesc _texture_desc;
        _texture_desc.Name = "uiw user texture";
        _texture_desc.Type = Diligent::RESOURCE_DIM_TEX_2D;
        _texture_desc.Width = static_cast<Diligent::Uint32>(width);
        _texture_desc.Height = static_cast<Diligent::Uint32>(height);
        _texture_desc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM;
        _texture_desc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
        _texture_desc.Usage = Diligent::USAGE_IMMUTABLE;
        Diligent::TextureSubResData _texture_subres_data[] = { { pixels.data(), 4 * Diligent::Uint64 { _texture_desc.Width } } };
        Diligent::TextureData _texture_data(_texture_subres_data, _countof(_texture_subres_data));
        _global_render_device->CreateTexture(_texture_desc, &_texture_data, &_diligent_texture);
        _diligent_texture_view = _diligent_texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        _has_value = true;
    }

    void* texture_handle::get() const
    {
        if (_has_value) {
            return (ImTextureID)(_diligent_texture_view);
        }
        return nullptr;
    }

    // bool texture_handle::has_value() const
    // {
    //     return _has_value;
    // }

    // void texture_handle::reset()
    // {
    //     if (_has_value) {
    //         _diligent_texture.Release();
    //         _diligent_texture_view.Release();
    //         _has_value = false;
    //     }
    // }

}
}