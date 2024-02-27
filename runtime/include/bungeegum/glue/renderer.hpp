#pragma once

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#include <Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/GraphicsEngine/interface/Buffer.h>
#include <Graphics/GraphicsEngine/interface/DepthStencilState.h>
#include <Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Graphics/GraphicsEngine/interface/SwapChain.h>
#include <Graphics/GraphicsEngine/interface/TextureView.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <vector>

#include <bungeegum/config/backend.hpp>
#include <bungeegum/config/misc.hpp>
#include <bungeegum/core/math.hpp>
#include <bungeegum/glue/window.hpp>

struct ImGuiContext;
struct ImPlotContext;
struct ImFont;
struct ImFontAtlas;

namespace bungeegum {
namespace detail {

    /// @brief
    /// @param main_function
    /// @param position_alias
    /// @param color_alias
    /// @param texcoord_alias
    /// @param sample_alias
    /// @return
    [[nodiscard]] std::string hlsl_fragment(
        const std::string& main_function,
        const std::string& position_alias = "UIW_POSITION",
        const std::string& color_alias = "UIW_COLOR",
        const std::string& texcoord_alias = "UIW_TEXCOORD",
        const std::string& sample_alias = "UIW_SAMPLE");

    /// @brief
    /// @return
    [[nodiscard]] std::string hlsl_fragment_default();

    /// @brief Instances of this struct represent cross-platform GPU renderers that will select the
    /// most appropriate graphics API depending on the platform. Macros defined above can be
    /// modified to force usage of a specific platform.
    /// @details Instances of this struct can only be moved.
    struct renderer_handle {
        renderer_handle() = default;
        renderer_handle(const renderer_handle& other) = delete;
        renderer_handle& operator=(const renderer_handle& other) = delete;
        renderer_handle(renderer_handle&& other) = default;
        renderer_handle& operator=(renderer_handle&& other) = default;

#if BUNGEEGUM_USE_DIRECTX

        /// @brief Creates an instance from an existing DirectX 11 context.
        void emplace_attach_directx11(window& existing_window, void* directx_context, void* directx_swapchain);

        /// @brief Creates an instance from an existing DirectX 12 context.
        void emplace_attach_directx12(window& existing_window, void* directx_context, void* directx_swapchain);

#endif

        /// @brief Creates an instance from an existing OpenGL context.
        void emplace_attach_opengl(window& existing_window);

#if BUNGEEGUM_USE_VULKAN

        /// @brief Creates an instance from an existing window with the Vulkan API.
        void emplace_attach_vulkan(window& existing_window);

#endif

#if BUNGEEGUM_USE_DIRECTX

        /// @brief Creates an instance from an existing window with the DirectX 11 API.
        void emplace_create_directx11(window& existing_window);

        /// @brief Creates an instance from an existing window with the DirectX 12 API.
        void emplace_create_directx12(window& existing_window);

#endif

        /// @brief Creates an instance from an existing window with the OpenGL API.
        void emplace_create_opengl(window& existing_window);

#if BUNGEEGUM_USE_VULKAN

        /// @brief Creates an instance from an existing window with the Vulkan API.
        void emplace_create_vulkan(window& existing_window);

#endif

        /// @brief
        /// @return
        [[nodiscard]] bool has_value() const;

        /// @brief
        void reset();

        /// @brief Begins a new frame, enabling all drawing commands.
        void clear_screen(
            const bool clear_color_buffer = true,
            const bool clear_depth_buffer = true);

        /// @brief Ends the frame, disabling all drawing commands. Swaps the window buffers.
        void present();

        /// @brief
        /// @param display_size
        void resize(const float2 display_size);

        /// @brief
        /// @param must_use
        void use_color_buffer(const bool must_use);

        /// @brief The color to use when clearing the screen.
        float4 clear_color = { 1.f, 1.f, 1.f, 1.f };

    private:
        bool _has_value = false;
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> _diligent_render_device = {};
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> _diligent_device_context = {};
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> _diligent_swap_chain = {};
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        SDL_Window* _sdl_window = nullptr;
#endif
        friend struct rasterizer_handle;
    };

	struct rasterizer_handle;

    ///
    struct font_handle {

        /// @brief
        /// @param rasterizer
        /// @param ttf
        /// @param count
        /// @param size
        void emplace(
            rasterizer_handle& rasterizer,
            const void* ttf,
            const std::size_t count,
            const float size);

        /// @brief
        /// @return
        [[nodiscard]] ImFont* get() const;

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        void reset();

    private:
        bool _has_value = false;
        ImFont* _imgui_font = nullptr;
    };

    /// @brief
    struct texture_handle {
        texture_handle() = default;
        texture_handle(const texture_handle& other) = default;
        texture_handle& operator=(const texture_handle& other) = default;
        texture_handle(texture_handle&& other) = default;
        texture_handle& operator=(texture_handle&& other) = default;

        /// @brief
        /// @param rasterizer
        /// @param pixels
        /// @param width
        /// @param height
        void emplace(
            rasterizer_handle& rasterizer,
            const std::vector<unsigned char>& pixels,
            const std::size_t width,
            const std::size_t height);

        /// @brief
        /// @return
        [[nodiscard]] void* get() const;

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        void reset();

    private:
        bool _has_value = false;
        Diligent::RefCntAutoPtr<Diligent::ITexture> _diligent_texture = {};
        Diligent::RefCntAutoPtr<Diligent::ITextureView> _diligent_texture_view = {};
    };

    /// @brief
    struct shader_blend_descriptor {

        /// @brief
        bool enable = true;

        /// @brief
        Diligent::BLEND_FACTOR src = Diligent::BLEND_FACTOR_SRC_ALPHA;

        /// @brief
        Diligent::BLEND_FACTOR dest = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;

        /// @brief
        Diligent::BLEND_OPERATION op = Diligent::BLEND_OPERATION_ADD;

        /// @brief
        Diligent::BLEND_FACTOR src_alpha = Diligent::BLEND_FACTOR_INV_SRC_ALPHA;

        /// @brief
        Diligent::BLEND_FACTOR dest_alpha = Diligent::BLEND_FACTOR_ZERO;

        /// @brief
        Diligent::BLEND_OPERATION alpha_op = Diligent::BLEND_OPERATION_ADD;

        /// @brief
        Diligent::COLOR_MASK color_mask = Diligent::COLOR_MASK_ALL;
    };

    /// @brief
    struct shader_depth_descriptor {

        /// @brief
        bool enable = true;

        /// @brief
        bool enable_write = false;

        /// @brief
        Diligent::COMPARISON_FUNCTION function = Diligent::COMPARISON_FUNC_LESS;
    };

    /// @brief
    struct shader_stencil_descriptor {

        /// @brief
        bool enable = true;

        /// @brief
        std::uint8_t read_mask = 0xFF;

        /// @brief
        std::uint8_t write_mask = 0xFF;

        /// @brief
        Diligent::STENCIL_OP fail_op = Diligent::STENCIL_OP_KEEP;

        /// @brief
        Diligent::STENCIL_OP depth_fail_op = Diligent::STENCIL_OP_KEEP;

        /// @brief
        Diligent::STENCIL_OP pass_op = Diligent::STENCIL_OP_KEEP;

        /// @brief
        Diligent::COMPARISON_FUNCTION function = Diligent::COMPARISON_FUNC_ALWAYS;
    };

    /// @brief for user defined shaders in HLSL etc
    struct shader_handle {
        shader_handle() = default;
        shader_handle(const shader_handle& other) = default;
        shader_handle& operator=(const shader_handle& other) = default;
        shader_handle(shader_handle&& other) = default;
        shader_handle& operator=(shader_handle&& other) = default;

        /// @brief
        /// @param rasterizer
        /// @param fragment
        /// @param blend
        /// @param depth
        /// @param stencil
        void emplace(
            rasterizer_handle& rasterizer,
            const std::string& fragment,
            const shader_blend_descriptor& blend = {},
            const shader_depth_descriptor& depth = {},
            const shader_stencil_descriptor& stencil = {});

        /// @brief PERMET DE PASSER LE SHADER DANS UN VOID*
        /// @param data
        void emplace(void* data);

        /// @brief
        /// @return
        [[nodiscard]] void* get() const;

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        void reset();

        /// @brief
        /// @tparam value_t
        /// @param value
        template <typename value_t>
        void uniform(const value_t& value);

    private:
        bool _has_value = false;
        Diligent::RefCntAutoPtr<Diligent::IPipelineState> _diligent_pipeline_state = {};
        friend struct rasterizer_handle;
    };

    /// @brief
    struct rasterizer_handle {
        rasterizer_handle() = default;
        rasterizer_handle(const rasterizer_handle& other) = default;
        rasterizer_handle& operator=(const rasterizer_handle& other) = default;
        rasterizer_handle(rasterizer_handle&& other) = default;
        rasterizer_handle& operator=(rasterizer_handle&& other) = default;

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

        void consume_emscripten_mouse_events(std::vector<emscripten_mouse_event>& events);

        void consume_emscripten_wheel_events(std::vector<emscripten_wheel_event>& events);

        void consume_emscripten_key_events(std::vector<emscripten_key_event>& events);

#else

        void consume_sdl_events(std::vector<SDL_Event>& events);

#endif

        /// @brief
        /// @param renderer
        void emplace(renderer_handle& renderer, ImFontAtlas* atlas = nullptr);

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        void reset();

        /// @brief
        void new_frame();

        /// @brief
        void render();

        /// @brief
        /// @param index
        void use_shader(const shader_handle& shader);

        /// @brief
        void use_projection_orthographic();

        /// @brief
        /// @param fov
        void use_projection_perspective(const float fov);

    private:
        bool _has_value = false;
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> _diligent_render_device = {};
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> _diligent_device_context = {};
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> _diligent_swap_chain = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_vertex_buffer = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_index_buffer = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_uniform_buffer = {};
        Diligent::RefCntAutoPtr<Diligent::IPipelineState> _diligent_pipeline_state = {};
        Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> _diligent_shader_resource = {};
        Diligent::IShaderResourceVariable* _diligent_texture_variable = nullptr;
        ImGuiContext* _imgui_context = nullptr;
        ImPlotContext* _implot_context = nullptr;
        texture_handle _font_texture = {};
        bool _is_base_vertex_supported = false;
        float2 _display_position = {};
        float2 _display_size = {};
        float4x4 _projection_matrix = {};
        std::size_t _vertex_buffer_size = BUNGEEGUM_USE_INITIAL_VERTICES_SIZE;
        std::size_t _index_buffer_size = BUNGEEGUM_USE_INITIAL_INDICES_SIZE;
        friend struct font_handle;
        friend struct texture_handle;
        friend struct shader_handle;
    };
}
}
