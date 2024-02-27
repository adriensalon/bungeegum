#include <imgui.h>

#include <bungeegum/glue/toolchain.hpp>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/key_codes.h>
#else
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#endif

#include <implot.h>

#include <bungeegum/glue/renderer.hpp>


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

#if BUNGEEGUM_USE_OPENGL
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#endif

#if BUNGEEGUM_USE_DIRECTX
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#endif

#if BUNGEEGUM_USE_VULKAN
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>
#endif

#include <Graphics/GraphicsTools/interface/MapHelper.hpp>


#include <array>

#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/raw.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        constexpr std::string_view default_imgui_backend_name = "bungeegum backend";

        constexpr std::string_view default_vertex_shader_source = R"(
            cbuffer Constants
            {
                float4x4 ProjectionMatrix;
            }

            struct VSInput
            {
                float2 pos : ATTRIB0;
                float2 uv  : ATTRIB1;
                float4 col : ATTRIB2;
            };

            struct PSInput
            {
                float4 pos : SV_POSITION;
                float4 col : COLOR;
                float2 uv  : TEXCOORD;
            };

            void main(in VSInput VSIn, out PSInput PSIn)
            {
                PSIn.pos = mul(ProjectionMatrix, float4(VSIn.pos.xy, 0.0, 1.0));
                PSIn.col = VSIn.col;
                PSIn.uv  = VSIn.uv;
            }
            )";

        std::string string_replace(
            const std::string& source,
            const std::string& to_replace,
            const std::string& replace_with)
        {
            std::size_t _pos = 0;
            std::size_t _cursor = 0;
            std::size_t _rep_len = to_replace.length();
            std::stringstream _builder;
            do {
                _pos = source.find(to_replace, _cursor);
                if (std::string::npos != _pos) {
                    _builder << source.substr(_cursor, _pos - _cursor);
                    _builder << replace_with;
                    _cursor = _pos + _rep_len;
                }
            } while (std::string::npos != _pos);
            _builder << source.substr(_cursor);
            return (_builder.str());
        }
    }

    std::string hlsl_fragment(
        const std::string& main_function,
        const std::string& position_alias,
        const std::string& color_alias,
        const std::string& texcoord_alias,
        const std::string& sample_alias)
    {
        constexpr std::string_view _prefix = R"(
            struct  PSInput
            {
                float4 pos : SV_POSITION;
                float4 col : COLOR;
                float2 uv : TEXCOORD;
            };

            Texture2D Texture;
            SamplerState Texture_sampler;

            float4 main(in PSInput PSIn) : SV_Target
            {
            )";
        constexpr std::string_view _suffix = R"(
            })";
        std::string _main_function_replaced { main_function };
        _main_function_replaced = string_replace(_main_function_replaced, position_alias, "PSIn.pos");
        _main_function_replaced = string_replace(_main_function_replaced, color_alias, "PSIn.col");
        _main_function_replaced = string_replace(_main_function_replaced, texcoord_alias, "PSIn.uv");
        _main_function_replaced = string_replace(_main_function_replaced, sample_alias + "(", "Texture.Sample(Texture_sampler, ");
        return std::string(_prefix.data()) + _main_function_replaced + std::string(_suffix.data());
    }

    std::string hlsl_fragment_default()
    {
        constexpr std::string_view _fragment = R"(
            struct  PSInput
            {
                float4 pos : SV_POSITION;
                float4 col : COLOR;
                float2 uv  : TEXCOORD;
            };

            Texture2D    Texture;
            SamplerState Texture_sampler;

            float4 main(in PSInput PSIn) : SV_Target
            {
                return PSIn.col * Texture.Sample(Texture_sampler, PSIn.uv);
            }
            )";
        return _fragment.data();
    }

#if BUNGEEGUM_USE_DIRECTX

    void renderer_handle::emplace_create_directx11(window& existing_window)
    {
        Diligent::SwapChainDesc _swap_chain_descriptor;
        _swap_chain_descriptor.DefaultStencilValue = 0u;
        Diligent::IEngineFactoryD3D11* _factory_ptr = Diligent::GetEngineFactoryD3D11();
        Diligent::EngineD3D11CreateInfo _engine_create_info;
        // #if defined(__DEBUG__)
        //         _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
        // #endif
        _factory_ptr->CreateDeviceAndContextsD3D11(_engine_create_info, &_diligent_render_device, &_diligent_device_context);
        Diligent::Win32NativeWindow _win32_native_window(existing_window.get_native());
        _factory_ptr->CreateSwapChainD3D11(_diligent_render_device, _diligent_device_context, _swap_chain_descriptor,
            Diligent::FullScreenModeDesc {}, _win32_native_window, &_diligent_swap_chain);

        _sdl_window = existing_window.get_sdl();
    }

    void renderer_handle::emplace_create_directx12(window& existing_window)
    {
        Diligent::SwapChainDesc _swap_chain_descriptor;
        _swap_chain_descriptor.DefaultStencilValue = 0u;
        Diligent::Win32NativeWindow _win32_native_window(existing_window.get_native());

        // todo
    }

#endif

    void renderer_handle::emplace_create_opengl(window& existing_window)
    {
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        // _data->sdl_window = existing_window->get_sdl();
#endif
        Diligent::SwapChainDesc _swap_chain_descriptor;
        _swap_chain_descriptor.DefaultStencilValue = 0u;
        Diligent::IEngineFactoryOpenGL* _factory = Diligent::GetEngineFactoryOpenGL();
        Diligent::EngineGLCreateInfo _engine_create_info;
        // _engine_create_info.GraphicsAPIVersion = Diligent::Version(3, 1); marche meme pas
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _engine_create_info.Window = Diligent::EmscriptenNativeWindow("#canvas");
#else
        _engine_create_info.Window = Diligent::NativeWindow(existing_window.get_native());
#endif
        _factory->CreateDeviceAndSwapChainGL(
            _engine_create_info,
            &_diligent_render_device,
            &_diligent_device_context,
            _swap_chain_descriptor,
            &_diligent_swap_chain);
    }

#if BUNGEEGUM_USE_VULKAN
    
    void renderer_handle::emplace_create_vulkan(window& existing)
    {
        (void)existing;
    }

#endif

    bool renderer_handle::has_value() const
    {
        // return _data.operator bool();
        return false;
    }

    void renderer_handle::reset() 
    {

    }

    void renderer_handle::clear_screen(
        const bool clear_color_buffer, 
        const bool clear_depth_buffer)
    {
        (void)clear_color_buffer;
        (void)clear_depth_buffer;
        Diligent::ITextureView* _rtv_ptr = _diligent_swap_chain->GetCurrentBackBufferRTV();
        Diligent::ITextureView* _dsv_ptr = _diligent_swap_chain->GetDepthBufferDSV();
        _diligent_device_context->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const std::array<float, 4> _clear_color_array = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
        _diligent_device_context->ClearRenderTarget(_rtv_ptr, _clear_color_array.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _diligent_device_context->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG | Diligent::CLEAR_STENCIL_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        // _diligent_device_context->SetStencilRef(8);
    }

    void renderer_handle::present()
    {
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _diligent_swap_chain->Present();
#endif
    }

    void renderer_handle::resize(const float2 display_size)
    {
        float2 _rounded = math::round(display_size);
        _diligent_swap_chain->Resize(
            static_cast<unsigned int>(_rounded.x),
            static_cast<unsigned int>(_rounded.y),
            Diligent::SURFACE_TRANSFORM_OPTIMAL);
    }

    void renderer_handle::use_color_buffer(const bool is_used)
    {
        Diligent::ITextureView* _dsv_ptr = _diligent_swap_chain->GetDepthBufferDSV();
        if (is_used) {
            Diligent::ITextureView* _rtv_ptr = _diligent_swap_chain->GetCurrentBackBufferRTV();
            _diligent_device_context->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        } else {
            _diligent_device_context->SetRenderTargets(0, nullptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        }
    }

    void font_handle::emplace(
        rasterizer_handle& rasterizer,
        const void* ttf,
        const std::size_t count,
        const float size)
    {
        if (!rasterizer.has_value()) {
            throw backtraced_exception { "[rendering exception] impossible to create font because rasterizer has no value" };
        }
        reset();

        int _raw_width, _raw_height = 0;
        unsigned char* _raw_pixels = nullptr;
        ImGui::SetCurrentContext(rasterizer._imgui_context);
        ImGuiIO& _io = ImGui::GetIO();
        _imgui_font = _io.Fonts->AddFontFromMemoryCompressedTTF(ttf, static_cast<int>(count), size);

        // // font awesome for the glyphs
        // // ImFontConfig config;
        // // config.MergeMode = true;
        // // config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
        // // static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        // // icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(fa4_compressed_data, fa4_compressed_size, 13.0f, &config, icon_ranges);

        _io.Fonts->Build();
        _io.Fonts->GetTexDataAsRGBA32(&_raw_pixels, &_raw_width, &_raw_height);
        rasterizer._font_texture.emplace(
            rasterizer,
            std::vector<unsigned char>(_raw_pixels, _raw_pixels + (4 * _raw_width * _raw_height)),
            static_cast<std::size_t>(_raw_width),
            static_cast<std::size_t>(_raw_height));
        _io.Fonts->TexID = rasterizer._font_texture.get();
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

    void texture_handle::emplace(
        rasterizer_handle& rasterizer,
        const std::vector<unsigned char>& pixels,
        const std::size_t width,
        const std::size_t height)
    {
        if (!rasterizer.has_value()) {
            throw backtraced_exception { "[rendering exception] impossible to create texture because rasterizer has no value" };
        }
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
        rasterizer._diligent_render_device->CreateTexture(_texture_desc, &_texture_data, &_diligent_texture);
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

    bool texture_handle::has_value() const
    {
        return _has_value;
    }

    void texture_handle::reset()
    {
        if (_has_value) {
            _diligent_texture.Release();
            _diligent_texture_view.Release();
            _has_value = false;
        }
    }

    void shader_handle::emplace(
        rasterizer_handle& rasterizer,
        const std::string& fragment,
        const shader_blend_descriptor& blend,
        const shader_depth_descriptor& depth,
        const shader_stencil_descriptor& stencil)
    {
        if (!rasterizer.has_value()) {
            throw backtraced_exception { "[rendering exception] impossible to create shader because rasterizer has no value" };
        }
        reset();

        Diligent::ShaderCreateInfo _shader_create_info;
        _shader_create_info.Desc.UseCombinedTextureSamplers = true;

        Diligent::RefCntAutoPtr<Diligent::IShader> _vertex_shader;
        _shader_create_info.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
        _shader_create_info.Desc.Name = "uiw vertex shader";
        _shader_create_info.Source = default_vertex_shader_source.data();
        _shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE::SHADER_SOURCE_LANGUAGE_HLSL;
        rasterizer._diligent_render_device->CreateShader(_shader_create_info, &_vertex_shader);

        Diligent::RefCntAutoPtr<Diligent::IShader> _pixel_shader;
        _shader_create_info.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
        _shader_create_info.Desc.Name = "uiw fragment shader";
        _shader_create_info.Source = fragment.c_str();
        _shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE::SHADER_SOURCE_LANGUAGE_HLSL;
        rasterizer._diligent_render_device->CreateShader(_shader_create_info, &_pixel_shader);

        Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;

        PSOCreateInfo.PSODesc.Name = "uiw pso";
        Diligent::GraphicsPipelineDesc& GraphicsPipeline = PSOCreateInfo.GraphicsPipeline;

        const Diligent::SwapChainDesc& _swapchain_descriptpr = rasterizer._diligent_swap_chain->GetDesc();

        GraphicsPipeline.NumRenderTargets = 1;
        GraphicsPipeline.RTVFormats[0] = _swapchain_descriptpr.ColorBufferFormat;
        GraphicsPipeline.DSVFormat = _swapchain_descriptpr.DepthBufferFormat;
        GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        PSOCreateInfo.pVS = _vertex_shader;
        PSOCreateInfo.pPS = _pixel_shader;

        GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
        GraphicsPipeline.RasterizerDesc.ScissorEnable = true;

        GraphicsPipeline.DepthStencilDesc.DepthEnable = depth.enable;
        GraphicsPipeline.DepthStencilDesc.DepthWriteEnable = depth.enable_write;
        GraphicsPipeline.DepthStencilDesc.DepthFunc = depth.function;

        GraphicsPipeline.DepthStencilDesc.StencilEnable = stencil.enable;
        GraphicsPipeline.DepthStencilDesc.StencilReadMask = stencil.read_mask;
        GraphicsPipeline.DepthStencilDesc.StencilWriteMask = stencil.write_mask;
        GraphicsPipeline.DepthStencilDesc.FrontFace.StencilFailOp = stencil.fail_op;
        GraphicsPipeline.DepthStencilDesc.FrontFace.StencilDepthFailOp = stencil.depth_fail_op;
        GraphicsPipeline.DepthStencilDesc.FrontFace.StencilPassOp = stencil.pass_op;
        GraphicsPipeline.DepthStencilDesc.FrontFace.StencilFunc = stencil.function;
        GraphicsPipeline.DepthStencilDesc.BackFace.StencilFailOp = stencil.fail_op;
        GraphicsPipeline.DepthStencilDesc.BackFace.StencilDepthFailOp = stencil.depth_fail_op;
        GraphicsPipeline.DepthStencilDesc.BackFace.StencilPassOp = stencil.pass_op;
        GraphicsPipeline.DepthStencilDesc.BackFace.StencilFunc = stencil.function;

        Diligent::RenderTargetBlendDesc& _blend_desc = GraphicsPipeline.BlendDesc.RenderTargets[0];
        _blend_desc.BlendEnable = blend.enable;
        _blend_desc.SrcBlend = blend.src;
        _blend_desc.DestBlend = blend.dest;
        _blend_desc.BlendOp = blend.op;
        _blend_desc.SrcBlendAlpha = blend.src_alpha;
        _blend_desc.DestBlendAlpha = blend.dest_alpha;
        _blend_desc.BlendOpAlpha = blend.alpha_op;
        _blend_desc.RenderTargetWriteMask = blend.color_mask;

        Diligent::LayoutElement VSInputs[] {
            { 0, 0, 2, Diligent::VT_FLOAT32 }, // pos
            { 1, 0, 2, Diligent::VT_FLOAT32 }, // uv
            { 2, 0, 4, Diligent::VT_UINT8, true } // col
        };
        GraphicsPipeline.InputLayout.NumElements = _countof(VSInputs);
        GraphicsPipeline.InputLayout.LayoutElements = VSInputs;

        Diligent::ShaderResourceVariableDesc Variables[] = {
            { Diligent::SHADER_TYPE_PIXEL, "Texture", Diligent::SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC } //
        };
        PSOCreateInfo.PSODesc.ResourceLayout.Variables = Variables;
        PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Variables);

        Diligent::SamplerDesc SamLinearWrap;
        SamLinearWrap.AddressU = Diligent::TEXTURE_ADDRESS_WRAP;
        SamLinearWrap.AddressV = Diligent::TEXTURE_ADDRESS_WRAP;
        SamLinearWrap.AddressW = Diligent::TEXTURE_ADDRESS_WRAP;
        Diligent::ImmutableSamplerDesc ImtblSamplers[] = {
            { Diligent::SHADER_TYPE_PIXEL, "Texture", SamLinearWrap } //
        };
        PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
        PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

        // create pipeline state
        rasterizer._diligent_render_device->CreateGraphicsPipelineState(PSOCreateInfo, &(_diligent_pipeline_state));

        // bind uniform buffer from rasterizer
        _diligent_pipeline_state->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "Constants")->Set(rasterizer._diligent_uniform_buffer);

        _has_value = true;
    }

    void shader_handle::emplace(void* data)
    {
        if (!data) {
            throw backtraced_exception { "[rendering exception] impossible to create shader because rasterizer data is nullptr" };
        }
        reset();
        _diligent_pipeline_state = reinterpret_cast<Diligent::IPipelineState*>(data);
        _has_value = true;
    }

    void* shader_handle::get() const
    {
        if (_has_value) {
            return static_cast<void*>(_diligent_pipeline_state.RawPtr());
        }
        return nullptr;
    }

    bool shader_handle::has_value() const
    {
        return _has_value;
    }

    void shader_handle::reset()
    {
        if (_has_value) {
            _diligent_pipeline_state.Release();
            _has_value = false;
        }
    }

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

    void rasterizer_handle::consume_emscripten_key_events(std::vector<emscripten_key_event>& events)
    {
        ImGui::SetCurrentContext(_imgui_context);
        // for (const emscripten_key_event& _event : events) {
        //     _data->imgui_renderer->OnKeyEvent(_event.event_type, _event.event);
        // }
    }

    void rasterizer_handle::consume_emscripten_mouse_events(std::vector<emscripten_mouse_event>& events)
    {
        ImGui::SetCurrentContext(_imgui_context);
        auto& io = ImGui::GetIO();
        for (const emscripten_mouse_event& _event : events) {
            io.MousePos = ImVec2(_event.event->targetX, _event.event->targetY);
            io.MouseDown[0] = _event.event->buttons & 1;
            io.MouseDown[1] = _event.event->buttons & 2;
            io.MouseDown[2] = _event.event->buttons & 4;
        }
    }

    void rasterizer_handle::consume_emscripten_wheel_events(std::vector<emscripten_wheel_event>& events)
    {
        ImGui::SetCurrentContext(_imgui_context);
        // for (const emscripten_wheel_event& _event : events) {
        //     _data->imgui_renderer->OnWheelEvent(_event.event_type, _event.event);
        // }
    }

#else

    void rasterizer_handle::consume_sdl_events(std::vector<SDL_Event>& events)
    {
        ImGui::SetCurrentContext(_imgui_context);
        for (const SDL_Event& _event : events) {
            ImGui_ImplSDL2_ProcessEvent(&_event);
        }
    }

#endif

    void rasterizer_handle::emplace(
        renderer_handle& renderer,
        ImFontAtlas* atlas)
    {
        if (_has_value) {
            reset();
        }

        _is_base_vertex_supported = renderer._diligent_render_device->GetAdapterInfo().DrawCommand.CapFlags & Diligent::DRAW_COMMAND_CAP_FLAG_BASE_VERTEX;
        _imgui_context = ImGui::CreateContext(atlas);
        _implot_context = ImPlot::CreateContext();
        ImGui::SetCurrentContext(_imgui_context);
        ImGuiIO& _io = ImGui::GetIO();
        // _io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        _io.IniFilename = nullptr;
        _io.BackendPlatformName = default_imgui_backend_name.data();
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _io.KeyMap[ImGuiKey_Tab] = DOM_VK_TAB;
        _io.KeyMap[ImGuiKey_LeftArrow] = DOM_VK_LEFT;
        _io.KeyMap[ImGuiKey_RightArrow] = DOM_VK_RIGHT;
        _io.KeyMap[ImGuiKey_UpArrow] = DOM_VK_UP;
        _io.KeyMap[ImGuiKey_DownArrow] = DOM_VK_DOWN;
        _io.KeyMap[ImGuiKey_PageUp] = DOM_VK_PAGE_UP;
        _io.KeyMap[ImGuiKey_PageDown] = DOM_VK_PAGE_DOWN;
        _io.KeyMap[ImGuiKey_Home] = DOM_VK_HOME;
        _io.KeyMap[ImGuiKey_End] = DOM_VK_END;
        _io.KeyMap[ImGuiKey_Delete] = DOM_VK_DELETE;
        _io.KeyMap[ImGuiKey_Backspace] = DOM_VK_BACK_SPACE;
        _io.KeyMap[ImGuiKey_Enter] = DOM_VK_ENTER;
        _io.KeyMap[ImGuiKey_Escape] = DOM_VK_ESCAPE;
        _io.KeyMap[ImGuiKey_A] = DOM_VK_A;
        _io.KeyMap[ImGuiKey_C] = DOM_VK_C;
        _io.KeyMap[ImGuiKey_V] = DOM_VK_V;
        _io.KeyMap[ImGuiKey_X] = DOM_VK_X;
        _io.KeyMap[ImGuiKey_Y] = DOM_VK_Y;
        _io.KeyMap[ImGuiKey_Z] = DOM_VK_Z;
#else
        ImGui_ImplSDL2_InitForOpenGL(renderer._sdl_window, nullptr);
#endif
        _diligent_device_context = renderer._diligent_device_context;
        _diligent_render_device = renderer._diligent_render_device;
        _diligent_swap_chain = renderer._diligent_swap_chain;

        // create uniform buffer
        {
            Diligent::BufferDesc _diligent_uniform_buffer_desc;
            _diligent_uniform_buffer_desc.Size = sizeof(float4x4);
            _diligent_uniform_buffer_desc.Usage = Diligent::USAGE_DYNAMIC;
            _diligent_uniform_buffer_desc.BindFlags = Diligent::BIND_UNIFORM_BUFFER;
            _diligent_uniform_buffer_desc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
            _diligent_render_device->CreateBuffer(_diligent_uniform_buffer_desc, nullptr, &_diligent_uniform_buffer);
        }

        _io.Fonts->AddFontDefault();
        _io.Fonts->Build();
        int _raw_width, _raw_height = 0;
        unsigned char* _raw_pixels = nullptr;
        _io.Fonts->GetTexDataAsRGBA32(&_raw_pixels, &_raw_width, &_raw_height);

        _has_value = true; // we do it now because required for creating texture
        _font_texture.emplace(
            *this,
            std::vector<unsigned char>(_raw_pixels, _raw_pixels + (4 * _raw_width * _raw_height)),
            static_cast<std::size_t>(_raw_width),
            static_cast<std::size_t>(_raw_height));
        _io.Fonts->TexID = _font_texture.get();
    }

    void rasterizer_handle::reset()
    {
        ImGui::DestroyContext(_imgui_context);
        ImPlot::DestroyContext(_implot_context);
        _diligent_device_context.Release();
        _diligent_render_device.Release();
        _diligent_swap_chain.Release();
        _diligent_vertex_buffer.Release();
        _diligent_index_buffer.Release();
        _diligent_uniform_buffer.Release();
        _font_texture.reset();
        _diligent_shader_resource.Release();
        _diligent_texture_variable = nullptr;
        _has_value = false;
    }

    bool rasterizer_handle::has_value() const
    {
        return _has_value;
    }

    void rasterizer_handle::new_frame()
    {
        ImGui::SetCurrentContext(_imgui_context);
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        const Diligent::SwapChainDesc& _swapchain_descriptor = _diligent_swap_chain->GetDesc();
        const float _display_height = static_cast<float>(_swapchain_descriptor.Height);
        const float _display_width = static_cast<float>(_swapchain_descriptor.Width);

        static auto m_LastTimestamp = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed_ns = now - m_LastTimestamp;
        m_LastTimestamp = now;
        auto& io = ImGui::GetIO();
        io.DeltaTime = static_cast<float>(elapsed_ns.count() / 1e+9);
        io.DisplaySize = ImVec2(_display_width, _display_height);
#else
        ImGui_ImplSDL2_NewFrame();
#endif
        ImGui::NewFrame();
    }

    void rasterizer_handle::render()
    {
        ImGui::Render();
        const ImDrawData* _imgui_draw_data = ImGui::GetDrawData();

        // Avoid rendering when minimized
        if (_imgui_draw_data->DisplaySize.x <= 0.0f || _imgui_draw_data->DisplaySize.y <= 0.0f)
            return;

        // Create and grow vertex/index buffers if needed
        if (!_diligent_vertex_buffer || static_cast<int>(_vertex_buffer_size) < _imgui_draw_data->TotalVtxCount) {
            _diligent_vertex_buffer.Release();
            while (static_cast<int>(_vertex_buffer_size) < _imgui_draw_data->TotalVtxCount) {
                _vertex_buffer_size *= 2;
            }
            Diligent::BufferDesc VBDesc;
            VBDesc.Name = "Imgui vertex buffer";
            VBDesc.BindFlags = Diligent::BIND_VERTEX_BUFFER;
            VBDesc.Size = _vertex_buffer_size * sizeof(ImDrawVert);
            VBDesc.Usage = Diligent::USAGE_DYNAMIC;
            VBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
            _diligent_render_device->CreateBuffer(VBDesc, nullptr, &(_diligent_vertex_buffer));
        }
        if (!_diligent_index_buffer || static_cast<int>(_index_buffer_size) < _imgui_draw_data->TotalIdxCount) {
            _diligent_index_buffer.Release();
            while (static_cast<int>(_index_buffer_size) < _imgui_draw_data->TotalIdxCount) {
                _index_buffer_size *= 2;
            }
            Diligent::BufferDesc IBDesc;
            IBDesc.Name = "Imgui index buffer";
            IBDesc.BindFlags = Diligent::BIND_INDEX_BUFFER;
            IBDesc.Size = _index_buffer_size * sizeof(ImDrawIdx);
            IBDesc.Usage = Diligent::USAGE_DYNAMIC;
            IBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
            _diligent_render_device->CreateBuffer(IBDesc, nullptr, &(_diligent_index_buffer));
        }

        {
            Diligent::MapHelper<ImDrawVert> Vertices(_diligent_device_context, _diligent_vertex_buffer, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
            Diligent::MapHelper<ImDrawIdx> Indices(_diligent_device_context, _diligent_index_buffer, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
            ImDrawVert* pVtxDst = Vertices;
            ImDrawIdx* pIdxDst = Indices;
            for (int CmdListID = 0; CmdListID < _imgui_draw_data->CmdListsCount; CmdListID++) {
                const ImDrawList* pCmdList = _imgui_draw_data->CmdLists[CmdListID];
                memcpy(pVtxDst, pCmdList->VtxBuffer.Data, pCmdList->VtxBuffer.Size * sizeof(ImDrawVert));
                memcpy(pIdxDst, pCmdList->IdxBuffer.Data, pCmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
                pVtxDst += pCmdList->VtxBuffer.Size;
                pIdxDst += pCmdList->IdxBuffer.Size;
            }
        }

        // ONCE PER FRAME OK

        // Setup orthographic projection matrix into our constant buffer
        // Our visible imgui space lies from _imgui_draw_data->DisplayPos (top left) to _imgui_draw_data->DisplayPos+data_data->DisplaySize (bottom right).
        // DisplayPos is (0,0) for single viewport apps.
        // {
        //     // DisplaySize always refers to the logical dimensions that account for pre-transform, hence
        //     // the aspect ratio will be correct after applying appropriate rotation.
        //     float L = _imgui_draw_data->DisplayPos.x;
        //     float R = _imgui_draw_data->DisplayPos.x + _imgui_draw_data->DisplaySize.x;
        //     float T = _imgui_draw_data->DisplayPos.y;
        //     float B = _imgui_draw_data->DisplayPos.y + _imgui_draw_data->DisplaySize.y;

        //     _projection_matrix = float4x4 {
        //         2.0f / (R - L), 0.0f, 0.0f, 0.0f,
        //         0.0f, 2.0f / (T - B), 0.0f, 0.0f,
        //         0.0f, 0.0f, 0.5f, 0.0f,
        //         (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f
        //     };
        //     Diligent::MapHelper<float4x4> CBData(_diligent_device_context, _diligent_uniform_buffer, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
        //     *CBData = _projection_matrix;
        // }
        _display_position = float2 { _imgui_draw_data->DisplayPos.x, _imgui_draw_data->DisplayPos.y };
        _display_size = float2 { _imgui_draw_data->DisplaySize.x, _imgui_draw_data->DisplaySize.y };

        auto SetupRenderState = [&]() //
        {
            // Setup shader and vertex buffers
            Diligent::IBuffer* pVBs[] = { _diligent_vertex_buffer };
            _diligent_device_context->SetVertexBuffers(0, 1, pVBs, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
            _diligent_device_context->SetIndexBuffer(_diligent_index_buffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            _diligent_device_context->SetPipelineState(_diligent_pipeline_state);

            const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
            _diligent_device_context->SetBlendFactors(blend_factor);

            Diligent::Viewport vp;
            vp.Width = static_cast<float>(_imgui_draw_data->DisplaySize.x) * _imgui_draw_data->FramebufferScale.x;
            vp.Height = static_cast<float>(_imgui_draw_data->DisplaySize.y) * _imgui_draw_data->FramebufferScale.y;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = vp.TopLeftY = 0;
            _diligent_device_context->SetViewports(1,
                &vp,
                static_cast<Diligent::Uint32>(_imgui_draw_data->DisplaySize.x * _imgui_draw_data->FramebufferScale.x),
                static_cast<Diligent::Uint32>(_imgui_draw_data->DisplaySize.y * _imgui_draw_data->FramebufferScale.y));
        };

        SetupRenderState();

        // Render command lists
        // (Because we merged all buffers into a single one, we maintain our own offset into them)
        Diligent::Uint32 GlobalIdxOffset = 0;
        Diligent::Uint32 GlobalVtxOffset = 0;

        Diligent::ITextureView* pLastTextureView = nullptr;
        for (int CmdListID = 0; CmdListID < _imgui_draw_data->CmdListsCount; CmdListID++) {
            const ImDrawList* pCmdList = _imgui_draw_data->CmdLists[CmdListID];
            for (int CmdID = 0; CmdID < pCmdList->CmdBuffer.Size; CmdID++) {
                const ImDrawCmd* pCmd = &pCmdList->CmdBuffer[CmdID];
                if (pCmd->UserCallback != NULL) {
                    // User callback, registered via ImDrawList::AddCallback()
                    // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                    if (pCmd->UserCallback == ImDrawCallback_ResetRenderState)
                        SetupRenderState();
                    else
                        pCmd->UserCallback(pCmdList, pCmd);
                } else {
                    // Apply scissor/clipping rectangle
                    float4 ClipRect //
                        {
                            (pCmd->ClipRect.x - _imgui_draw_data->DisplayPos.x) * _imgui_draw_data->FramebufferScale.x,
                            (pCmd->ClipRect.y - _imgui_draw_data->DisplayPos.y) * _imgui_draw_data->FramebufferScale.y,
                            (pCmd->ClipRect.z - _imgui_draw_data->DisplayPos.x) * _imgui_draw_data->FramebufferScale.x,
                            (pCmd->ClipRect.w - _imgui_draw_data->DisplayPos.y) * _imgui_draw_data->FramebufferScale.y //
                        };
                    // Apply pretransform
                    // ClipRect = TransformClipRect(_imgui_draw_data->DisplaySize, ClipRect);

                    Diligent::Rect Scissor //
                        {
                            static_cast<Diligent::Int32>(ClipRect.x),
                            static_cast<Diligent::Int32>(ClipRect.y),
                            static_cast<Diligent::Int32>(ClipRect.z),
                            static_cast<Diligent::Int32>(ClipRect.w) //
                        };
                    _diligent_device_context->SetScissorRects(1,
                        &Scissor,
                        static_cast<Diligent::Uint32>(_imgui_draw_data->DisplaySize.x * _imgui_draw_data->FramebufferScale.x),
                        static_cast<Diligent::Uint32>(_imgui_draw_data->DisplaySize.y * _imgui_draw_data->FramebufferScale.y));

                    // Bind texture
                    auto* pTextureView = reinterpret_cast<Diligent::ITextureView*>(pCmd->TextureId);
                    VERIFY_EXPR(pTextureView);
                    // if (pTextureView != pLastTextureView) {
					(void)pLastTextureView;
                    pLastTextureView = pTextureView;
                    _diligent_texture_variable->Set(pTextureView);
                    // _diligent_texture_variable->Set(_diligent_swap_chain->GetDepthBufferDSV()->GetTexture()->GetDefaultView(Diligent::TEXTURE_VIEW_TYPE::TEXTURE_VIEW_SHADER_RESOURCE));

                    _diligent_device_context->CommitShaderResources(_diligent_shader_resource, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                    // }

                    Diligent::DrawIndexedAttribs DrawAttrs { pCmd->ElemCount, sizeof(ImDrawIdx) == sizeof(Diligent::Uint16) ? Diligent::VT_UINT16 : Diligent::VT_UINT32, Diligent::DRAW_FLAG_VERIFY_STATES };
                    DrawAttrs.FirstIndexLocation = pCmd->IdxOffset + GlobalIdxOffset;
                    if (_is_base_vertex_supported) {
                        DrawAttrs.BaseVertex = pCmd->VtxOffset + GlobalVtxOffset;
                    } else {
                        Diligent::IBuffer* pVBs[] = { _diligent_vertex_buffer };
                        Diligent::Uint64 VtxOffsets[] = { sizeof(ImDrawVert) * (size_t { pCmd->VtxOffset } + size_t { GlobalVtxOffset }) };
                        _diligent_device_context->SetVertexBuffers(0, 1, pVBs, VtxOffsets, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_NONE);
                    }
                    _diligent_device_context->DrawIndexed(DrawAttrs);
                }
            }
            GlobalIdxOffset += pCmdList->IdxBuffer.Size;
            GlobalVtxOffset += pCmdList->VtxBuffer.Size;
        }
    }

    void rasterizer_handle::use_shader(const shader_handle& shader)
    {
        _diligent_pipeline_state = shader._diligent_pipeline_state;
        _diligent_device_context->SetPipelineState(_diligent_pipeline_state);

        // bind texture slot
        _diligent_shader_resource.Release();
        _diligent_pipeline_state->CreateShaderResourceBinding(&(_diligent_shader_resource), true);
        _diligent_texture_variable = _diligent_shader_resource->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "Texture");
        VERIFY_EXPR(_diligent_texture_variable != nullptr);

        // commit uniform matrix (without transform for now)
        Diligent::MapHelper<float4x4> _map_helper(_diligent_device_context, _diligent_uniform_buffer, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
        *_map_helper = _projection_matrix;
    }

    void rasterizer_handle::use_projection_orthographic()
    {
        float _left = _display_position.x;
        float _right = _display_position.x + _display_size.x;
        float _top = _display_position.y;
        float _bottom = _display_position.y + _display_size.y;
        _projection_matrix = float4x4 {
            2.0f / (_right - _left), 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f / (_top - _bottom), 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            (_right + _left) / (_left - _right), (_top + _bottom) / (_bottom - _top), 0.5f, 1.0f
        };
    }

    void rasterizer_handle::use_projection_perspective(const float fov)
    {
    }

}
}