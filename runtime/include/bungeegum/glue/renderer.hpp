#pragma once

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

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#include <Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/GraphicsEngine/interface/Buffer.h>
#include <Graphics/GraphicsEngine/interface/DepthStencilState.h>
#include <Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Graphics/GraphicsEngine/interface/GraphicsTypes.h>
#include <Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Graphics/GraphicsEngine/interface/SwapChain.h>
#include <Graphics/GraphicsEngine/interface/TextureView.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <bungeegum/config/backend.hpp>
#include <bungeegum/config/feature.hpp>
#include <bungeegum/core/math.hpp>
#include <bungeegum/glue/window.hpp>

namespace Diligent {
struct IRenderDevice;
struct IDeviceContext;
struct ISwapChain;
struct IPipelineState;
struct IBuffer;
struct ITexture;
struct ITextureView;
struct IShaderResourceVariable;
}

struct ImGuiContext;
struct ImPlotContext;
struct ImFont;
struct ImFontAtlas;

namespace bungeegum {
namespace detail {

    constexpr std::string_view default_imgui_backend_name = "bungeegum backend";
    constexpr unsigned int default_initial_vertex_buffer_size = 1024;
    constexpr unsigned int default_initial_index_buffer_size = 2048;

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
    /// @brief
    inline static std::string_view default_emscripten_canvas = "#canvas";
#endif







    /// @brief Instances of this struct represent cross-platform GPU renderers that will select the
    /// most appropriate graphics API depending on the platform. Macros defined above can be
    /// modified to force usage of a specific platform.
    /// @details Instances of this struct can only be moved.
    struct renderer {
        renderer() = default;
        renderer(const renderer& other) = delete;
        renderer& operator=(const renderer& other) = delete;
        renderer(renderer&& other) = default;
        renderer& operator=(renderer&& other) = default;

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
        /// @brief Creates an instance from an existing DirectX 11 context.
        void attach_directx11(window& existing_window, void* directx_context, void* directx_swapchain);

        /// @brief Creates an instance from an existing DirectX 12 context.
        void attach_directx12(window& existing_window, void* directx_context, void* directx_swapchain);
#endif

        /// @brief Creates an instance from an existing OpenGL context.
        void attach_opengl(window& existing_window);

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID)
        /// @brief Creates an instance from an existing window with the Vulkan API.
        void attach_vulkan(window& existing_window);
#endif

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
        /// @brief Creates an instance from an existing window with the DirectX 11 API.
        void create_directx11(window& existing_window);

        /// @brief Creates an instance from an existing window with the DirectX 12 API.
        void create_directx12(window& existing_window);
#endif

        /// @brief Creates an instance from an existing window with the OpenGL API.
        void create_opengl(window& existing_window);

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID)
        /// @brief Creates an instance from an existing window with the Vulkan API.
        void create_vulkan(window& existing_window);
#endif

        /// @brief
        void destroy();

        /// @brief
        /// @return
        [[nodiscard]] bool has_value() const;

        /// @brief Begins a new frame, enabling all drawing commands.
        /// @exception Throws a backtraced exception if a new frame has already begun.
        void clear_screen();

        /// @brief Ends the frame, disabling all drawing commands. Swaps the window buffers.
        /// @exception Throws a backtraced exception if the frame has already been ended.
        void present();

        /// @brief
        /// @param display_size
        void resize(const float2 display_size);

        /// @brief The color to use when clearing the screen.
        float4 clear_color = { 1.f, 1.f, 1.f, 1.f };

    private:
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice> _diligent_render_device;
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> _diligent_device_context;
        Diligent::RefCntAutoPtr<Diligent::ISwapChain> _diligent_swap_chain;
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
        SDL_Window* _sdl_window = nullptr;
#endif
        friend struct shader_handle;
        friend struct texture_handle;
        friend struct imgui_context_handle;
        friend struct imgui_font_handle;
        friend struct imgui_renderer;
    };





















    

    /// @brief
    struct texture_handle {
        texture_handle() = default;
        texture_handle(const texture_handle& other) = default;
        texture_handle& operator=(const texture_handle& other) = default;
        texture_handle(texture_handle&& other) = default;
        texture_handle& operator=(texture_handle&& other) = default;

        /// @brief
        /// @param owner
        /// @param pixels
        /// @param width
        /// @param height
        void create(
            renderer& owner,
            const std::vector<unsigned char>& pixels,
            const std::size_t width,
            const std::size_t height);

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        void destroy();

        /// @brief 
        /// @return 
        std::optional<void*> get_id() const;

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
        /// @param owner 
        /// @param fragment 
        /// @param blend 
        /// @param depth 
        /// @param stencil 
        void create(
            renderer& owner,
            const std::string& fragment,
            const shader_blend_descriptor& blend = { },
            const shader_depth_descriptor& depth = { },
            const shader_stencil_descriptor& stencil = {});
            

        /// @brief
        void destroy();

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        /// @tparam value_t
        /// @param value
        template <typename value_t>
        void uniform(const value_t& value);

        /// @brief
        void use() const;

    private:
        bool _has_value = false;
        std::optional<std::reference_wrapper<renderer>> _owner = std::nullopt; 
        Diligent::RefCntAutoPtr<Diligent::IPipelineState> _diligent_pipeline_state = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_vertex_buffer = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_index_buffer = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_uniform_buffer = {};
        friend struct imgui_context_handle;
        friend struct imgui_font_handle;
    };











    /// @brief
    struct imgui_context_handle {
        imgui_context_handle() = default;
        imgui_context_handle(const imgui_context_handle& other) = default;
        imgui_context_handle& operator=(const imgui_context_handle& other) = default;
        imgui_context_handle(imgui_context_handle&& other) = default;
        imgui_context_handle& operator=(imgui_context_handle&& other) = default;
     
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        void consume_emscripten_mouse_events(std::vector<emscripten_mouse_event>& events);

        void consume_emscripten_wheel_events(std::vector<emscripten_wheel_event>& events);

        void consume_emscripten_key_events(std::vector<emscripten_key_event>& events);
#else
        void consume_sdl_events(std::vector<SDL_Event>& events);
#endif

        /// @brief
        /// @param owner
        void create(renderer& owner, ImFontAtlas* atlas = nullptr);

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        void destroy();

        /// @brief
        void new_frame();

        /// @brief
        void render();        
        
        /// @brief
        texture_handle font_texture = {};
        
        /// @brief
        shader_handle default_shader = {};
        
        /// @brief
        shader_handle mask_shader = {};

        /// @brief 
        float4x4 projection_matrix; // default to orthographic!

        /// @brief 
        float4x4 view_matrix; // default to identity!

    private:
        bool _has_value = false;
        bool _is_base_vertex_supported = false;
        ImGuiContext* _imgui_context = nullptr;
        ImPlotContext* _implot_context = nullptr;
        std::optional<std::reference_wrapper<renderer>> _owner = std::nullopt;        
        unsigned int _vertex_buffer_size = default_initial_vertex_buffer_size;
        unsigned int _index_buffer_size = default_initial_index_buffer_size;
        Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> _diligent_shader_resource = {};
        Diligent::IShaderResourceVariable* _diligent_texture_variable = nullptr;
        friend struct imgui_font_handle;
    };

    ///
    struct imgui_font_handle {

        void create(
            renderer& owner,
            imgui_context_handle& context,
            const void* ttf,
            const std::size_t count,
            const float size);

        
        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        void destroy();  

        std::optional<ImFont*> get_font() const;

    private:
        bool _has_value = false;
        ImFont* _imgui_font = nullptr;
    };
}
}
