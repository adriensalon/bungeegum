// #define PLATFORM_WIN32
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/renderer.hpp> // TOOLCHAIN HERE !

#include <array>

#include <imgui.h>
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/html5.h>
#include <emscripten/key_codes.h>
#else
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#endif
#include <implot.h>

// Define those before including diligent headers
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

#include <Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h>
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
#include <Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#endif
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID)
#include <Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h>
#endif
// #include <Imgui/interface/ImGuiDiligentRenderer.hpp>
// #include <Imgui/interface/ImGuiImplDiligent.hpp>
// #if TOOLCHAIN_PLATFORM_EMSCRIPTEN
// #include <Imgui/interface/ImGuiImplEmscripten.hpp>
// #else
// #include <Imgui/interface/ImGuiImplSDL.hpp>
// #endif

static const char* VertexShaderHLSL = R"(
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

static const char* PixelShaderHLSL = R"(
struct PSInput
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

namespace bungeegum {
namespace detail {

    struct imgui_renderer {

        static void setup_context(window& wnd, renderer& rnd)
        {
            rnd._is_base_vertex_supported = rnd._diligent_render_device->GetAdapterInfo().DrawCommand.CapFlags & Diligent::DRAW_COMMAND_CAP_FLAG_BASE_VERTEX;
			// ImGui::GetIO().Fonts; // existing font atlas
            rnd._user_imgui_context = ImGui::CreateContext();
            rnd._implot_context = ImPlot::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.IniFilename = nullptr;
            io.BackendPlatformName = default_imgui_backend_name.data();
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
            io.KeyMap[ImGuiKey_Tab] = DOM_VK_TAB;
            io.KeyMap[ImGuiKey_LeftArrow] = DOM_VK_LEFT;
            io.KeyMap[ImGuiKey_RightArrow] = DOM_VK_RIGHT;
            io.KeyMap[ImGuiKey_UpArrow] = DOM_VK_UP;
            io.KeyMap[ImGuiKey_DownArrow] = DOM_VK_DOWN;
            io.KeyMap[ImGuiKey_PageUp] = DOM_VK_PAGE_UP;
            io.KeyMap[ImGuiKey_PageDown] = DOM_VK_PAGE_DOWN;
            io.KeyMap[ImGuiKey_Home] = DOM_VK_HOME;
            io.KeyMap[ImGuiKey_End] = DOM_VK_END;
            io.KeyMap[ImGuiKey_Delete] = DOM_VK_DELETE;
            io.KeyMap[ImGuiKey_Backspace] = DOM_VK_BACK_SPACE;
            io.KeyMap[ImGuiKey_Enter] = DOM_VK_ENTER;
            io.KeyMap[ImGuiKey_Escape] = DOM_VK_ESCAPE;
            io.KeyMap[ImGuiKey_A] = DOM_VK_A;
            io.KeyMap[ImGuiKey_C] = DOM_VK_C;
            io.KeyMap[ImGuiKey_V] = DOM_VK_V;
            io.KeyMap[ImGuiKey_X] = DOM_VK_X;
            io.KeyMap[ImGuiKey_Y] = DOM_VK_Y;
            io.KeyMap[ImGuiKey_Z] = DOM_VK_Z;
#else
			ImGui_ImplSDL2_InitForOpenGL(wnd.get_sdl(), nullptr);
#endif
        }

        static void invalidate_renderer(renderer& rnd)
        {
            rnd._diligent_ignore_stencil_vertex_buffer.Release();
            rnd._diligent_ignore_stencil_index_buffer.Release();
            rnd._diligent_uniform_buffer.Release();
            rnd._diligent_ignore_stencil_pipeline.Release();
            rnd._overlay_font_texture.Release();
            rnd._diligent_shader_resource.Release();

        }

        static void setup_renderer(renderer& rnd)
        {
            const Diligent::SwapChainDesc& _swapchain_descriptpr = rnd._diligent_swap_chain->GetDesc();

            invalidate_renderer(rnd);

            Diligent::ShaderCreateInfo _shader_create_info;
            // _shader_create_info.UseCombinedTextureSamplers = true;
            // _shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_DEFAULT;
            _shader_create_info.Desc.UseCombinedTextureSamplers = true;

            Diligent::RefCntAutoPtr<Diligent::IShader> _vertex_shader;
            _shader_create_info.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
            _shader_create_info.Desc.Name = "Imgui VS";
            _shader_create_info.Source = VertexShaderHLSL;
			_shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE::SHADER_SOURCE_LANGUAGE_HLSL;
            rnd._diligent_render_device->CreateShader(_shader_create_info, &_vertex_shader);

            Diligent::RefCntAutoPtr<Diligent::IShader> _pixel_shader;
            _shader_create_info.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
            _shader_create_info.Desc.Name = "Imgui PS";
            _shader_create_info.Source = PixelShaderHLSL;
			_shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE::SHADER_SOURCE_LANGUAGE_HLSL;
            rnd._diligent_render_device->CreateShader(_shader_create_info, &_pixel_shader);

            Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;

            PSOCreateInfo.PSODesc.Name = "ImGUI PSO";
            auto& GraphicsPipeline = PSOCreateInfo.GraphicsPipeline;

            GraphicsPipeline.NumRenderTargets = 1;
            GraphicsPipeline.RTVFormats[0] = _swapchain_descriptpr.ColorBufferFormat;
            GraphicsPipeline.DSVFormat = _swapchain_descriptpr.DepthBufferFormat;
            GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

            PSOCreateInfo.pVS = _vertex_shader;
            PSOCreateInfo.pPS = _pixel_shader;

            GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
            GraphicsPipeline.RasterizerDesc.ScissorEnable = true;
            GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
            GraphicsPipeline.DepthStencilDesc.DepthWriteEnable = false;
            GraphicsPipeline.DepthStencilDesc.StencilEnable = true;

            auto& RT0 = GraphicsPipeline.BlendDesc.RenderTargets[0];
            RT0.BlendEnable = true;
            RT0.SrcBlend = Diligent::BLEND_FACTOR_SRC_ALPHA;
            RT0.DestBlend = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
            RT0.BlendOp = Diligent::BLEND_OPERATION_ADD;
            RT0.SrcBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
            RT0.DestBlendAlpha = Diligent::BLEND_FACTOR_ZERO;
            RT0.BlendOpAlpha = Diligent::BLEND_OPERATION_ADD;
            RT0.RenderTargetWriteMask = Diligent::COLOR_MASK_ALL;

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

            rnd._diligent_render_device->CreateGraphicsPipelineState(PSOCreateInfo, &(rnd._diligent_ignore_stencil_pipeline));

            {
                Diligent::BufferDesc BuffDesc;
                BuffDesc.Size = sizeof(float4x4);
                BuffDesc.Usage = Diligent::USAGE_DYNAMIC;
                BuffDesc.BindFlags = Diligent::BIND_UNIFORM_BUFFER;
                BuffDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
                rnd._diligent_render_device->CreateBuffer(BuffDesc, nullptr, &(rnd._diligent_uniform_buffer));
            }
            rnd._diligent_ignore_stencil_pipeline->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "Constants")->Set(rnd._diligent_uniform_buffer);

            // setup_fonts_texture(rnd);
        }

        static void new_frame(renderer& rnd)
        {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
            const Diligent::SwapChainDesc& _swapchain_descriptor = rnd._diligent_swap_chain->GetDesc();
            const float _display_height = static_cast<float>(_swapchain_descriptor.Height);
            const float _display_width = static_cast<float>(_swapchain_descriptor.Width);

            // bof
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
            if (!rnd._diligent_ignore_stencil_pipeline) {
                setup_renderer(rnd);
            }
            ImGui::NewFrame();
            ImGui::SetCurrentContext(rnd._user_imgui_context); // ig on new frame is enough for now?
            ImPlot::SetCurrentContext(rnd._implot_context); // ig on new frame is enough for now?
        }

        static void render(renderer& rnd)
        {
            ImGui::Render();
            const ImDrawData* _imgui_draw_data = ImGui::GetDrawData();

            // Avoid rendering when minimized
            if (_imgui_draw_data->DisplaySize.x <= 0.0f || _imgui_draw_data->DisplaySize.y <= 0.0f)
                return;

            // Create and grow vertex/index buffers if needed
            if (!rnd._diligent_ignore_stencil_vertex_buffer || static_cast<int>(rnd._vertex_buffer_size) < _imgui_draw_data->TotalVtxCount) {
                rnd._diligent_ignore_stencil_vertex_buffer.Release();
                while (static_cast<int>(rnd._vertex_buffer_size) < _imgui_draw_data->TotalVtxCount) {
                    rnd._vertex_buffer_size *= 2;
                }
                Diligent::BufferDesc VBDesc;
                VBDesc.Name = "Imgui vertex buffer";
                VBDesc.BindFlags = Diligent::BIND_VERTEX_BUFFER;
                VBDesc.Size = rnd._vertex_buffer_size * sizeof(ImDrawVert);
                VBDesc.Usage = Diligent::USAGE_DYNAMIC;
                VBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
                rnd._diligent_render_device->CreateBuffer(VBDesc, nullptr, &(rnd._diligent_ignore_stencil_vertex_buffer));
            }
            if (!rnd._diligent_ignore_stencil_index_buffer || static_cast<int>(rnd._index_buffer_size) < _imgui_draw_data->TotalIdxCount) {
                rnd._diligent_ignore_stencil_index_buffer.Release();
                while (static_cast<int>(rnd._index_buffer_size) < _imgui_draw_data->TotalIdxCount) {
                    rnd._index_buffer_size *= 2;
                }
                Diligent::BufferDesc IBDesc;
                IBDesc.Name = "Imgui index buffer";
                IBDesc.BindFlags = Diligent::BIND_INDEX_BUFFER;
                IBDesc.Size = rnd._index_buffer_size * sizeof(ImDrawIdx);
                IBDesc.Usage = Diligent::USAGE_DYNAMIC;
                IBDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
                rnd._diligent_render_device->CreateBuffer(IBDesc, nullptr, &(rnd._diligent_ignore_stencil_index_buffer));
            }

            {
                Diligent::MapHelper<ImDrawVert> Vertices(rnd._diligent_device_context, rnd._diligent_ignore_stencil_vertex_buffer, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
                Diligent::MapHelper<ImDrawIdx> Indices(rnd._diligent_device_context, rnd._diligent_ignore_stencil_index_buffer, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
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

            // Setup orthographic projection matrix into our constant buffer
            // Our visible imgui space lies from _imgui_draw_data->DisplayPos (top left) to _imgui_draw_data->DisplayPos+data_data->DisplaySize (bottom right).
            // DisplayPos is (0,0) for single viewport apps.
            {
                // DisplaySize always refers to the logical dimensions that account for pre-transform, hence
                // the aspect ratio will be correct after applying appropriate rotation.
                float L = _imgui_draw_data->DisplayPos.x;
                float R = _imgui_draw_data->DisplayPos.x + _imgui_draw_data->DisplaySize.x;
                float T = _imgui_draw_data->DisplayPos.y;
                float B = _imgui_draw_data->DisplayPos.y + _imgui_draw_data->DisplaySize.y;

                float4x4 Projection {
                    2.0f / (R - L), 0.0f, 0.0f, 0.0f,
                    0.0f, 2.0f / (T - B), 0.0f, 0.0f,
                    0.0f, 0.0f, 0.5f, 0.0f,
                    (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f
                };
                Diligent::MapHelper<float4x4> CBData(rnd._diligent_device_context, rnd._diligent_uniform_buffer, Diligent::MAP_WRITE, Diligent::MAP_FLAG_DISCARD);
                *CBData = Projection;
            }

            auto SetupRenderState = [&]() //
            {
                // Setup shader and vertex buffers
                Diligent::IBuffer* pVBs[] = { rnd._diligent_ignore_stencil_vertex_buffer };
                rnd._diligent_device_context->SetVertexBuffers(0, 1, pVBs, nullptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
                rnd._diligent_device_context->SetIndexBuffer(rnd._diligent_ignore_stencil_index_buffer, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                rnd._diligent_device_context->SetPipelineState(rnd._diligent_ignore_stencil_pipeline);

                const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
                rnd._diligent_device_context->SetBlendFactors(blend_factor);

                Diligent::Viewport vp;
                vp.Width = static_cast<float>(_imgui_draw_data->DisplaySize.x) * _imgui_draw_data->FramebufferScale.x;
                vp.Height = static_cast<float>(_imgui_draw_data->DisplaySize.y) * _imgui_draw_data->FramebufferScale.y;
                vp.MinDepth = 0.0f;
                vp.MaxDepth = 1.0f;
                vp.TopLeftX = vp.TopLeftY = 0;
                rnd._diligent_device_context->SetViewports(1,
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
                        rnd._diligent_device_context->SetScissorRects(1,
                            &Scissor,
                            static_cast<Diligent::Uint32>(_imgui_draw_data->DisplaySize.x * _imgui_draw_data->FramebufferScale.x),
                            static_cast<Diligent::Uint32>(_imgui_draw_data->DisplaySize.y * _imgui_draw_data->FramebufferScale.y));

                        // Bind texture
                        auto* pTextureView = reinterpret_cast<Diligent::ITextureView*>(pCmd->TextureId);
                        VERIFY_EXPR(pTextureView);
                        if (pTextureView != pLastTextureView) {
                            pLastTextureView = pTextureView;
                            rnd._diligent_texture_variable->Set(pTextureView);
                            rnd._diligent_device_context->CommitShaderResources(rnd._diligent_shader_resource, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
                        }

                        Diligent::DrawIndexedAttribs DrawAttrs { pCmd->ElemCount, sizeof(ImDrawIdx) == sizeof(Diligent::Uint16) ? Diligent::VT_UINT16 : Diligent::VT_UINT32, Diligent::DRAW_FLAG_VERIFY_STATES };
                        DrawAttrs.FirstIndexLocation = pCmd->IdxOffset + GlobalIdxOffset;
                        if (rnd._is_base_vertex_supported) {
                            DrawAttrs.BaseVertex = pCmd->VtxOffset + GlobalVtxOffset;
                        } else {
                            Diligent::IBuffer* pVBs[] = { rnd._diligent_ignore_stencil_vertex_buffer };
                            Diligent::Uint64 VtxOffsets[] = { sizeof(ImDrawVert) * (size_t { pCmd->VtxOffset } + size_t { GlobalVtxOffset }) };
                            rnd._diligent_device_context->SetVertexBuffers(0, 1, pVBs, VtxOffsets, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, Diligent::SET_VERTEX_BUFFERS_FLAG_NONE);
                        }
                        rnd._diligent_device_context->DrawIndexed(DrawAttrs);
                    }
                }
                GlobalIdxOffset += pCmdList->IdxBuffer.Size;
                GlobalVtxOffset += pCmdList->VtxBuffer.Size;
            }
        }

		static void setup_user_fonts_texture(renderer& rnd)
		{
			_setup_fonts_texture(rnd, rnd._user_font_texture);
		}

#if BUNGEEGUM_USE_OVERLAY
		static void setup_overlay_fonts_texture(renderer& rnd)
		{
			_setup_fonts_texture(rnd, rnd._overlay_font_texture);
		}
#endif

	private:
        static void _setup_fonts_texture(renderer& rnd, Diligent::RefCntAutoPtr<Diligent::ITextureView>& texture_view)
        {
            // Build texture atlas
            ImGuiIO& IO = ImGui::GetIO();

            unsigned char* pData = nullptr;
            int Width = 0;
            int Weight = 0;
            IO.Fonts->GetTexDataAsRGBA32(&pData, &Width, &Weight);

            Diligent::TextureDesc FontTexDesc;
            FontTexDesc.Name = "Imgui font texture";
            FontTexDesc.Type = Diligent::RESOURCE_DIM_TEX_2D;
            FontTexDesc.Width = static_cast<Diligent::Uint32>(Width);
            FontTexDesc.Height = static_cast<Diligent::Uint32>(Weight);
            FontTexDesc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM;
            FontTexDesc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
            FontTexDesc.Usage = Diligent::USAGE_IMMUTABLE;

            Diligent::TextureSubResData Mip0Data[] = { { pData, 4 * Diligent::Uint64 { FontTexDesc.Width } } };
            Diligent::TextureData InitData(Mip0Data, _countof(Mip0Data));

            Diligent::RefCntAutoPtr<Diligent::ITexture> pFontTex;
            rnd._diligent_render_device->CreateTexture(FontTexDesc, &InitData, &pFontTex);
            texture_view = pFontTex->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
            rnd._diligent_shader_resource.Release();
            rnd._diligent_ignore_stencil_pipeline->CreateShaderResourceBinding(&(rnd._diligent_shader_resource), true);
            rnd._diligent_texture_variable = rnd._diligent_shader_resource->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "Texture");
            VERIFY_EXPR(rnd._diligent_texture_variable != nullptr);

            // Store our identifier
            IO.Fonts->TexID = (ImTextureID)(texture_view);
        }

	};

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
    void renderer::create_directx11(window& existing_window)
    {
        Diligent::SwapChainDesc _swap_chain_descriptor;
        Diligent::IEngineFactoryD3D11* _factory_ptr = Diligent::GetEngineFactoryD3D11();
        Diligent::EngineD3D11CreateInfo _engine_create_info;
// #if defined(__DEBUG__)
//         _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
// #endif
        _factory_ptr->CreateDeviceAndContextsD3D11(_engine_create_info, &_diligent_render_device, &_diligent_device_context);
        Diligent::Win32NativeWindow _win32_native_window(existing_window.get_native());
        _factory_ptr->CreateSwapChainD3D11(_diligent_render_device, _diligent_device_context, _swap_chain_descriptor,
            Diligent::FullScreenModeDesc {}, _win32_native_window, &_diligent_swap_chain);
        
        imgui_renderer::setup_context(existing_window, *this);
        imgui_renderer::setup_renderer(*this);
    }

    void renderer::create_directx12(window& existing_window)
    {
        Diligent::SwapChainDesc _swap_chain_descriptor;
        Diligent::Win32NativeWindow _win32_native_window(existing_window.get_native());

        // todo

        imgui_renderer::setup_context(existing_window, *this);
        imgui_renderer::setup_renderer(*this);
    }
#endif

    void renderer::create_opengl(window& existing_window)
    {
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        // _data->sdl_window = existing_window->get_sdl();
#endif
        Diligent::SwapChainDesc _swap_chain_descriptor;
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
        imgui_renderer::setup_context(existing_window, *this);
        imgui_renderer::setup_renderer(*this);
    }

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID)
    void renderer::create_vulkan(window& existing)
    {
        (void)existing;
    }
#endif

    void renderer::rebuild_user_fonts()
    {
        imgui_renderer::setup_user_fonts_texture(*this);
    }

#if BUNGEEGUM_USE_OVERLAY
    void renderer::rebuild_overlay_fonts()
    {
        imgui_renderer::setup_overlay_fonts_texture(*this);
    }
#endif

    bool renderer::has_value() const
    {
        // return _data.operator bool();
		return false;
    }

    void renderer::new_frame()
    {
        Diligent::ITextureView* _rtv_ptr = _diligent_swap_chain->GetCurrentBackBufferRTV();
        Diligent::ITextureView* _dsv_ptr = _diligent_swap_chain->GetDepthBufferDSV();
        _diligent_device_context->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        const std::array<float, 4> _clear_color_array = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
        _diligent_device_context->ClearRenderTarget(_rtv_ptr, _clear_color_array.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _diligent_device_context->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        imgui_renderer::new_frame(*this);
    }

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    void renderer::consume_emscripten_key_events(std::vector<emscripten_key_event>& events)
    {
        // for (const emscripten_key_event& _event : events) {
        //     _data->imgui_renderer->OnKeyEvent(_event.event_type, _event.event);
        // }
        events.clear();
    }

    void renderer::consume_emscripten_mouse_events(std::vector<emscripten_mouse_event>& events)
    {
        auto& io = ImGui::GetIO();        
        for (const emscripten_mouse_event& _event : events) {
			io.MousePos = ImVec2(_event.event->targetX, _event.event->targetY);
            io.MouseDown[0] = _event.event->buttons & 1;
            io.MouseDown[1] = _event.event->buttons & 2;
            io.MouseDown[2] = _event.event->buttons & 4;
        }
        events.clear();
    }

    void renderer::consume_emscripten_wheel_events(std::vector<emscripten_wheel_event>& events)
    {
        // for (const emscripten_wheel_event& _event : events) {
        //     _data->imgui_renderer->OnWheelEvent(_event.event_type, _event.event);
        // }
        events.clear();
    }
#else
    void renderer::consume_sdl_events(std::vector<SDL_Event>& events)
    {
        for (const SDL_Event& _event : events) {
            // _data->imgui_renderer->OnEvent(_event);
			
			if (ImGui::GetCurrentContext() == NULL)
				return;

			ImGui_ImplSDL2_ProcessEvent(&_event);
        }
        events.clear();
    }
#endif

    void renderer::present()
    {
        imgui_renderer::render(*this);
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _diligent_swap_chain->Present();
#endif
    }

    void renderer::resize(const float2 display_size)
    {
        float2 _rounded = math::round(display_size);
        _diligent_swap_chain->Resize(
            static_cast<unsigned int>(_rounded.x),
            static_cast<unsigned int>(_rounded.y),
            Diligent::SURFACE_TRANSFORM_OPTIMAL);
    }

	void texture_handle::create(
		renderer& owner, 
		const std::vector<unsigned char>& pixels,
		const std::size_t width,
		const std::size_t height)
	{
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
        owner._diligent_render_device->CreateTexture(_texture_desc, &_texture_data, &_diligent_texture);
	}

	
	void shader_handle::create(
		renderer& owner,
		const std::string& vertex,
		const std::string& fragment /* TODO */)
	{
		Diligent::ShaderCreateInfo _shader_create_info;
		// _shader_create_info.UseCombinedTextureSamplers = true;
		// _shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_DEFAULT;
		_shader_create_info.Desc.UseCombinedTextureSamplers = true;

		Diligent::RefCntAutoPtr<Diligent::IShader> _vertex_shader;
		_shader_create_info.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
		_shader_create_info.Desc.Name = "uiw vertex shader";
		// _shader_create_info.Source = vertex.c_str();
		_shader_create_info.Source = VertexShaderHLSL;
		_shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE::SHADER_SOURCE_LANGUAGE_HLSL;
		owner._diligent_render_device->CreateShader(_shader_create_info, &_vertex_shader);

		Diligent::RefCntAutoPtr<Diligent::IShader> _pixel_shader;
		_shader_create_info.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
		_shader_create_info.Desc.Name = "uiw fragment shader";
		_shader_create_info.Source = fragment.c_str();
		_shader_create_info.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE::SHADER_SOURCE_LANGUAGE_HLSL;
		owner._diligent_render_device->CreateShader(_shader_create_info, &_pixel_shader);

		Diligent::GraphicsPipelineStateCreateInfo PSOCreateInfo;

		PSOCreateInfo.PSODesc.Name = "uiw pso";
		auto& GraphicsPipeline = PSOCreateInfo.GraphicsPipeline;

		const Diligent::SwapChainDesc& _swapchain_descriptpr = owner._diligent_swap_chain->GetDesc();

		GraphicsPipeline.NumRenderTargets = 1;
		GraphicsPipeline.RTVFormats[0] = _swapchain_descriptpr.ColorBufferFormat;
		GraphicsPipeline.DSVFormat = _swapchain_descriptpr.DepthBufferFormat;
		GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		PSOCreateInfo.pVS = _vertex_shader;
		PSOCreateInfo.pPS = _pixel_shader;

		GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
		GraphicsPipeline.RasterizerDesc.ScissorEnable = true;
		GraphicsPipeline.DepthStencilDesc.DepthEnable = true;
		GraphicsPipeline.DepthStencilDesc.DepthWriteEnable = false;
		GraphicsPipeline.DepthStencilDesc.StencilEnable = true;

		auto& RT0 = GraphicsPipeline.BlendDesc.RenderTargets[0];
		RT0.BlendEnable = true;
		RT0.SrcBlend = Diligent::BLEND_FACTOR_SRC_ALPHA;
		RT0.DestBlend = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
		RT0.BlendOp = Diligent::BLEND_OPERATION_ADD;
		RT0.SrcBlendAlpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;
		RT0.DestBlendAlpha = Diligent::BLEND_FACTOR_ZERO;
		RT0.BlendOpAlpha = Diligent::BLEND_OPERATION_ADD;
		RT0.RenderTargetWriteMask = Diligent::COLOR_MASK_ALL;

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

		owner._diligent_render_device->CreateGraphicsPipelineState(PSOCreateInfo, &(_diligent_pipeline_state));

		{
			Diligent::BufferDesc BuffDesc;
			BuffDesc.Size = sizeof(float4x4);
			BuffDesc.Usage = Diligent::USAGE_DYNAMIC;
			BuffDesc.BindFlags = Diligent::BIND_UNIFORM_BUFFER;
			BuffDesc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;
			owner._diligent_render_device->CreateBuffer(BuffDesc, nullptr, &(_diligent_uniform_buffer));
		}
		_diligent_pipeline_state->GetStaticVariableByName(Diligent::SHADER_TYPE_VERTEX, "Constants")->Set(_diligent_uniform_buffer);

	}

	
	void imgui_font_handle::create(
		imgui_shader_handle& owner,
		const void* ttf, 
		const std::size_t count,
		const float size)
	{
		ImGui::SetCurrentContext(owner._user_imgui_context);
		ImGuiIO& _io = ImGui::GetIO();
		_io.Fonts->AddFontFromMemoryCompressedTTF(ttf, static_cast<int>(count), size);
		_io.Fonts->Build();
		// update font texture on owner
	}
	
}
}