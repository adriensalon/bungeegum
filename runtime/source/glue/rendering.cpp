#include <bungeegum/glue/rendering.hpp>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/key_codes.h>
#else
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#endif

#include <imgui.h>
#include <implot.h>

#include <stb_image.h>


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
        _main_function_replaced = string_replace(_main_function_replaced, position_alias, "PSIn.pos");
        _main_function_replaced = string_replace(_main_function_replaced, color_alias, "PSIn.col");
        _main_function_replaced = string_replace(_main_function_replaced, texcoord_alias, "PSIn.uv");
        _main_function_replaced = string_replace(_main_function_replaced, sample_alias + "(", "Texture.Sample(Texture_sampler, ");
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
    
    

    struct renderer_handle::renderer_data {
        bool _has_value = false;
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> _diligent_render_device = {};
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> _diligent_device_context = {};
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> _diligent_swap_chain = {};
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        SDL_Window* _sdl_window = nullptr;
#endif
    };

#if BUNGEEGUM_USE_DIRECTX

    renderer_handle renderer_handle::emplace_attach_directx11(window_handle& window, void* device, void* context)
    {
// #if !defined(__HOTRELOADING__)
//         if (_has_value) {
//             throw backtraced_exception("[rendering] Impossible to emplace renderer because this instance already has a value");
//         }
//         Diligent::IEngineFactoryD3D11* _factory_ptr = Diligent::GetEngineFactoryD3D11();
//         Diligent::EngineD3D11CreateInfo _engine_create_info;
//         // #if defined(__DEBUG__)
//         //         _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
//         // #endif
//         _factory_ptr->AttachToD3D11Device(device, context, _engine_create_info, &_diligent_render_device, &_diligent_device_context);
//         Diligent::Win32NativeWindow _win32_native_window(window.get_native());        
//         Diligent::SwapChainDesc _swap_chain_descriptor;
//         _swap_chain_descriptor.DefaultStencilValue = 0u;
//         _factory_ptr->CreateSwapChainD3D11(
//             _diligent_render_device, 
//             _diligent_device_context, 
//             _swap_chain_descriptor,
//             Diligent::FullScreenModeDesc {}, 
//             _win32_native_window, 
//             &_diligent_swap_chain);

//         _sdl_window = window.get_sdl();
// 		_has_value = true;
// #endif
        return {};
    }

    renderer_handle renderer_handle::emplace_new_directx11(window_handle& window)
    {
        renderer_handle _renderer;
        _renderer._data = std::make_shared<renderer_data>();
        
        Diligent::SwapChainDesc _swap_chain_descriptor;
        _swap_chain_descriptor.DefaultStencilValue = 0u;
        Diligent::IEngineFactoryD3D11* _factory_ptr = Diligent::GetEngineFactoryD3D11();
        Diligent::EngineD3D11CreateInfo _engine_create_info;
        // #if defined(__DEBUG__)
        //         _engine_create_info.SetValidationLevel(Diligent::VALIDATION_LEVEL_2);
        // #endif
        _factory_ptr->CreateDeviceAndContextsD3D11(_engine_create_info, &_renderer._data->_diligent_render_device, &_renderer._data->_diligent_device_context);
        Diligent::Win32NativeWindow _win32_native_window(window.get_native());
        _factory_ptr->CreateSwapChainD3D11(_renderer._data->_diligent_render_device, _renderer._data->_diligent_device_context, _swap_chain_descriptor,
            Diligent::FullScreenModeDesc {}, _win32_native_window, &_renderer._data->_diligent_swap_chain);

        _renderer._data->_sdl_window = window.get_sdl();
		_renderer._data->_has_value = true;
        return _renderer;
    }

    renderer_handle renderer_handle::emplace_new_directx12(window_handle& existing_window)
    {
        // if (_data->_has_value) {
        //     throw backtraced_exception("[rendering] Impossible to emplace renderer because this instance already has a value");
        // }
        // Diligent::SwapChainDesc _swap_chain_descriptor;
        // _swap_chain_descriptor.DefaultStencilValue = 0u;
        // Diligent::Win32NativeWindow _win32_native_window(existing_window.get_native());

        // // todo
		// _data->_has_value = true;
        return {};
    }

#endif

    renderer_handle renderer_handle::emplace_attach_opengl(window_handle& existing_window)
    {
//         if (_has_value) {
//             throw backtraced_exception("[rendering] Impossible to emplace renderer because this instance already has a value");
//         }
// #if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
//         // _data->sdl_window = existing_window->get_sdl();
// #endif
//         // Diligent::SwapChainDesc _swap_chain_descriptor;
//         // _swap_chain_descriptor.DefaultStencilValue = 0u;
//         Diligent::IEngineFactoryOpenGL* _factory = Diligent::GetEngineFactoryOpenGL();
//         Diligent::EngineGLCreateInfo _engine_create_info;
//         // _engine_create_info.GraphicsAPIVersion = Diligent::Version(3, 1); marche meme pas
// #if TOOLCHAIN_PLATFORM_EMSCRIPTEN
//         _engine_create_info.Window = Diligent::EmscriptenNativeWindow("#canvas");
// #else
//         _engine_create_info.Window = Diligent::NativeWindow(existing_window.get_native());
// #endif
//         _factory->AttachToActiveGLContext(
//             _engine_create_info,
//             &_diligent_render_device,
//             &_diligent_device_context);
//         // swapchain mdr?
// 		_has_value = true;
        return {};
    }

    renderer_handle renderer_handle::emplace_new_opengl(window_handle& existing_window)
    {
//         if (_has_value) {
//             throw backtraced_exception("[rendering] Impossible to emplace renderer because this instance already has a value");
//         }
// #if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
//         // _data->sdl_window = existing_window->get_sdl();
// #endif
//         Diligent::SwapChainDesc _swap_chain_descriptor;
//         _swap_chain_descriptor.DefaultStencilValue = 0u;
//         Diligent::IEngineFactoryOpenGL* _factory = Diligent::GetEngineFactoryOpenGL();
//         Diligent::EngineGLCreateInfo _engine_create_info;
//         // _engine_create_info.GraphicsAPIVersion = Diligent::Version(3, 1); marche meme pas
// #if TOOLCHAIN_PLATFORM_EMSCRIPTEN
//         _engine_create_info.Window = Diligent::EmscriptenNativeWindow("#canvas");
// #else
//         _engine_create_info.Window = Diligent::NativeWindow(existing_window.get_native());
// #endif
//         _factory->CreateDeviceAndSwapChainGL(
//             _engine_create_info,
//             &_diligent_render_device,
//             &_diligent_device_context,
//             _swap_chain_descriptor,
//             &_diligent_swap_chain);
// 		_has_value = true;
        return {};
    }

#if BUNGEEGUM_USE_VULKAN
    
    renderer_handle renderer_handle::emplace_new_vulkan(window_handle& existing)
    {
        // if (_data->_has_value) {
        //     throw backtraced_exception("[rendering] Impossible to emplace renderer because this instance already has a value");
        // }
        // (void)existing;
		// _data->_has_value = true;
        return {};
    }

#endif

    bool renderer_handle::has_value() const
    {
        return _data->_has_value;
    }

    void renderer_handle::reset() 
    {
		if (_data->_has_value) {
			
		}
    }

    void renderer_handle::clear_screen(
        const bool clear_color_buffer, 
        const bool clear_depth_buffer)
    {
        (void)clear_color_buffer;
        (void)clear_depth_buffer;
        Diligent::ITextureView* _rtv_ptr = _data->_diligent_swap_chain->GetCurrentBackBufferRTV();
        Diligent::ITextureView* _dsv_ptr = _data->_diligent_swap_chain->GetDepthBufferDSV();
        _data->_diligent_device_context->SetRenderTargets(1, &_rtv_ptr, _dsv_ptr, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        const std::array<float, 4> _clear_color_array = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
        _data->_diligent_device_context->ClearRenderTarget(_rtv_ptr, _clear_color_array.data(), Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        _data->_diligent_device_context->ClearDepthStencil(_dsv_ptr, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
        // _diligent_device_context->SetStencilRef(8);
    }
    
    rasterizer_handle renderer_handle::make_rasterizer()
    {
        rasterizer_handle _rasterizer;
        _rasterizer._diligent_render_device = _data->_diligent_render_device;
        _rasterizer._diligent_device_context = _data->_diligent_device_context;
        _rasterizer._diligent_swap_chain = _data->_diligent_swap_chain;

        _rasterizer._is_base_vertex_supported = _data->_diligent_render_device->GetAdapterInfo().DrawCommand.CapFlags & Diligent::DRAW_COMMAND_CAP_FLAG_BASE_VERTEX;
        _rasterizer._imgui_context = ImGui::CreateContext();
        _rasterizer._implot_context = ImPlot::CreateContext();
        ImGui::SetCurrentContext(_rasterizer._imgui_context);

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        ImGuiIO& _io = ImGui::GetIO();
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
        ImGui_ImplSDL2_InitForOpenGL(_data->_sdl_window, nullptr);
#endif
        return _rasterizer;
    }

    void renderer_handle::present()
    {
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        _data->_diligent_swap_chain->Present();
#endif
    }
	
    void renderer_handle::resize(const float2 display_size)
    {
        float2 _rounded = math::round(display_size);
        _data->_diligent_swap_chain->Resize(
            static_cast<unsigned int>(_rounded.x),
            static_cast<unsigned int>(_rounded.y),
            Diligent::SURFACE_TRANSFORM_OPTIMAL);
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



    void rasterizer_handle::new_frame()
    {
        ImGui::SetCurrentContext(_imgui_context);
        const Diligent::SwapChainDesc& _swapchain_descriptor = _diligent_swap_chain->GetDesc();
        const float _display_height = static_cast<float>(_swapchain_descriptor.Height);
        const float _display_width = static_cast<float>(_swapchain_descriptor.Width);
        ImGuiIO& _imgui_io = ImGui::GetIO();
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        static auto m_LastTimestamp = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed_ns = now - m_LastTimestamp;
        m_LastTimestamp = now;
        _imgui_io.DeltaTime = static_cast<float>(elapsed_ns.count() / 1e+9);
#else
        ImGui_ImplSDL2_NewFrame();
#endif
        _imgui_io.DisplaySize = ImVec2(_display_width, _display_height);
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

}
}