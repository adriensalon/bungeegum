#include <bungeegum/glue/rendering.hpp>
#include <bungeegum/glue/string.hpp>

#include <imgui.h>
#include <implot.h>

#include <stb_image.h>

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#endif

#include <Graphics/GraphicsTools/interface/MapHelper.hpp>

namespace bungeegum {
namespace detail {

    namespace {
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
    }

    std::string shader_fragment(
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
        _main_function_replaced = replace(_main_function_replaced, position_alias, "PSIn.pos");
        _main_function_replaced = replace(_main_function_replaced, color_alias, "PSIn.col");
        _main_function_replaced = replace(_main_function_replaced, texcoord_alias, "PSIn.uv");
        _main_function_replaced = replace(_main_function_replaced, sample_alias + "(", "Texture.Sample(Texture_sampler, ");
        return std::string(_prefix.data()) + _main_function_replaced + std::string(_suffix.data());
    }

    std::string shader_fragment_default()
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

    void font_handle::emplace(
        rasterizer_handle& rasterizer,
        const std::filesystem::path& filename,
        const float size,
        const std::optional<font_config>& config,
        unsigned short* ranges)
    {
        if (!rasterizer.has_value()) {
            throw backtraced_exception("glue/rendering", "Impossible to create font because rasterizer has no value.");
        }
        reset();

        int _raw_width, _raw_height = 0;
        unsigned char* _raw_pixels = nullptr;
        ImGui::SetCurrentContext(rasterizer._imgui_context);
        ImGuiIO& _io = ImGui::GetIO();

        std::string _spath = filename.generic_string();
        const char* _cpath = _spath.c_str();
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
        }
        _imgui_font = _io.Fonts->AddFontFromFileTTF(_cpath, size, &_imgui_config, ranges);

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

    void font_handle::emplace(
        rasterizer_handle& rasterizer,
        const void* ttf,
        const std::size_t count,
        const float size,
        const std::optional<font_config>& config,
        unsigned short* ranges)
    {
        if (!rasterizer.has_value()) {
            throw backtraced_exception("glue/rendering", "Impossible to create font because rasterizer has no value.");
        }
        reset();

        int _raw_width, _raw_height = 0;
        unsigned char* _raw_pixels = nullptr;
        ImGui::SetCurrentContext(rasterizer._imgui_context);
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
        const std::filesystem::path filename)
    {
        if (!rasterizer.has_value()) {
            throw backtraced_exception("glue/rendering", "Impossible to create texture because rasterizer has no value.");
        }
        reset();
        std::string _spath = filename.generic_string();
        const char* _cpath = _spath.c_str();
        int _width, _height, _channels;
        unsigned char* _data = stbi_load(_cpath, &_width, &_height, &_channels, 4);
        if (_channels < 3 || _channels > 4) {
            throw backtraced_exception("glue/rendering", "Impossible to create texture because channels count is invalid (expecting 3 or 4).");
        }
        std::vector<unsigned char> _pixels(_data, _data + 4 * _width * _height);
        stbi_image_free(_data);

        Diligent::TextureDesc _texture_desc;
        _texture_desc.Name = "uiw user texture";
        _texture_desc.Type = Diligent::RESOURCE_DIM_TEX_2D;
        _texture_desc.Width = static_cast<Diligent::Uint32>(_width);
        _texture_desc.Height = static_cast<Diligent::Uint32>(_height);
        _texture_desc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM;
        _texture_desc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
        _texture_desc.Usage = Diligent::USAGE_IMMUTABLE;
        Diligent::TextureSubResData _texture_subres_data[] = { { _pixels.data(), 4 * Diligent::Uint64 { _texture_desc.Width } } };
        Diligent::TextureData _texture_data(_texture_subres_data, _countof(_texture_subres_data));
        rasterizer._diligent_render_device->CreateTexture(_texture_desc, &_texture_data, &_diligent_texture);
        _diligent_texture_view = _diligent_texture->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        _has_value = true;
    }

    void texture_handle::emplace(
        rasterizer_handle& rasterizer,
        const std::vector<unsigned char>& pixels,
        const std::size_t width,
        const std::size_t height)
    {
        if (!rasterizer.has_value()) {
            throw backtraced_exception("glue/rendering", "Impossible to create texture because rasterizer has no value.");
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
            throw backtraced_exception("glue/rendering", "Impossible to create shader because rasterizer has no value.");
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
            throw backtraced_exception("glue/rendering", "Impossible to create shader because void data is nullptr.");
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

    void rasterizer_handle::use_shader(const shader_handle& shader)
    {
        _diligent_pipeline_state = shader._diligent_pipeline_state;
        _diligent_device_context->SetPipelineState(_diligent_pipeline_state);

        // bind texture slot
        _diligent_shader_resource.Release();
        _diligent_pipeline_state->CreateShaderResourceBinding(&(_diligent_shader_resource), true);
        _diligent_texture_variable = _diligent_shader_resource->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "Texture");
        // VERIFY_EXPR(_diligent_texture_variable != nullptr);

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