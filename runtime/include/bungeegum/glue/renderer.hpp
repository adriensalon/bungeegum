#pragma once

#include <memory>

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
#include <Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Graphics/GraphicsEngine/interface/SwapChain.h>
#include <Graphics/GraphicsEngine/interface/TextureView.h>

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

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        void consume_emscripten_mouse_events(std::vector<emscripten_mouse_event>& events);

        void consume_emscripten_wheel_events(std::vector<emscripten_wheel_event>& events);

        void consume_emscripten_key_events(std::vector<emscripten_key_event>& events);
#else
        void consume_sdl_events(std::vector<SDL_Event>& events);
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
        void new_frame();

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

    public:
        /// @brief Rebuilds the ImGui fonts.
        void rebuild_user_fonts();

#if BUNGEEGUM_USE_OVERLAY
        /// @brief Rebuilds the ImGui fonts.
        void rebuild_overlay_fonts();
#endif

        float4x4 projection_matrix; // default to orthographic!

        float4x4 view_matrix; // default to identity!

        Diligent::RefCntAutoPtr<Diligent::IPipelineState> _diligent_ignore_stencil_pipeline;
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_ignore_stencil_vertex_buffer;
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_ignore_stencil_index_buffer;
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_uniform_buffer;
        bool _is_base_vertex_supported = false;
        unsigned int _vertex_buffer_size = default_initial_vertex_buffer_size;
        unsigned int _index_buffer_size = default_initial_index_buffer_size;
        Diligent::RefCntAutoPtr<Diligent::ITextureView> _user_font_texture;
        Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> _diligent_shader_resource;
        Diligent::IShaderResourceVariable* _diligent_texture_variable = nullptr;
        ImGuiContext* _user_imgui_context = nullptr;
        ImPlotContext* _implot_context = nullptr;

#if BUNGEEGUM_USE_OVERLAY
        Diligent::RefCntAutoPtr<Diligent::ITextureView> _overlay_font_texture;
        ImGuiContext* _overlay_imgui_context = nullptr;
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
        /// @param owner
        void destroy(renderer& owner);

    private:
        Diligent::RefCntAutoPtr<Diligent::ITexture> _diligent_texture = {};
        Diligent::RefCntAutoPtr<Diligent::ITextureView> _diligent_texture_view = {};
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
        void create(
            renderer& owner,
            const std::string& vertex,
            const std::string& fragment /* TODO */);
        // le depth
        // le stenicl

        /// @brief
        /// @tparam value_t
        /// @param value
        template <typename value_t>
        void uniform(const value_t& value);

        /// @brief
        /// @param texture
        void uniform(const texture_handle& texture);

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        /// @param owner
        void destroy(renderer& owner);

    private:
        Diligent::RefCntAutoPtr<Diligent::IPipelineState> _diligent_pipeline_state = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_vertex_buffer = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_index_buffer = {};
        Diligent::RefCntAutoPtr<Diligent::IBuffer> _diligent_uniform_buffer = {};
    };

    /// @brief
    struct imgui_context_handle {
        imgui_context_handle() = default;
        imgui_context_handle(const imgui_context_handle& other) = default;
        imgui_context_handle& operator=(const imgui_context_handle& other) = default;
        imgui_context_handle(imgui_context_handle&& other) = default;
        imgui_context_handle& operator=(imgui_context_handle&& other) = default;

        /// @brief
        /// @param owner
        void create(
            renderer& owner,
            ImFontAtlas* atlas = nullptr);

        // fonts ?

        /// @brief
        [[nodiscard]] bool has_value() const;

        /// @brief
        /// @param owner
        void destroy(renderer& owner);

    private:
        bool _is_base_vertex_supported = false;
        unsigned int _vertex_buffer_size = default_initial_vertex_buffer_size;
        unsigned int _index_buffer_size = default_initial_index_buffer_size;
        Diligent::RefCntAutoPtr<Diligent::ITextureView> _user_font_texture;
        Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> _diligent_shader_resource;
        Diligent::IShaderResourceVariable* _diligent_texture_variable = nullptr;
        ImGuiContext* _user_imgui_context = nullptr;
        ImPlotContext* _implot_context = nullptr;
        friend struct imgui_font_handle;
    };

    ///
    struct imgui_font_handle {

        void create(
            imgui_context_handle& owner,
            const void* ttf,
            const std::size_t count,
            const float size);

    private:
        ImFont* _font_ptr = nullptr;
    };
}
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif