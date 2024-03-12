#pragma once

#include <memory>
#include <optional>

#include <bungeegum/core/simd.hpp>
#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/toolchain.hpp>
#include <bungeegum/glue/window.hpp>

/// @brief Defines if we use DirectX functionnality.
#if !defined(BUNGEEGUM_USE_DIRECTX)
#define BUNGEEGUM_USE_DIRECTX (BUNGEEGUM_ENABLE_DIRECTX && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP))
#endif

/// @brief Defines if we use Vulkan functionnality.
#if !defined(BUNGEEGUM_USE_VULKAN)
#define BUNGEEGUM_USE_VULKAN (BUNGEEGUM_ENABLE_VULKAN && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID))
#endif

/// @brief Defines if we use OpenGL functionnality.
#if !defined(BUNGEEGUM_USE_OPENGL)
#define BUNGEEGUM_USE_OPENGL (BUNGEEGUM_ENABLE_OPENGL && (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP || TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_ANDROID || TOOLCHAIN_PLATFORM_EMSCRIPTEN || TOOLCHAIN_PLATFORM_MACOS || TOOLCHAIN_PLATFORM_IOS))
#endif

namespace bungeegum {

/// @brief
enum struct renderer_backend {
#if BUNGEEGUM_USE_DIRECTX
    directx11,
    directx12,
#endif
#if BUNGEEGUM_USE_OPENGL
    opengl,
    opengles,
#endif
#if BUNGEEGUM_USE_VULKAN
    vulkan
#endif
};
}

#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
#define BUNGEEGUM_USE_PREFERRED_RENDERER_BACKEND renderer_backend::directx11
#elif TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_MACOS
#if BUNGEEGUM_USE_VULKAN
#define BUNGEEGUM_USE_PREFERRED_RENDERER_BACKEND renderer_backend::vulkan
#elif BUNGEEGUM_USE_OPENGL
#define BUNGEEGUM_USE_PREFERRED_RENDERER_BACKEND renderer_backend::opengl
#else
#error "Linux or MacOS but no vulkan nor opengl"
#endif
#elif TOOLCHAIN_PLATFORM_IOS || TOOLCHAIN_PLATFORM_EMSCRIPTEN
#if BUNGEEGUM_USE_OPENGL
#define BUNGEEGUM_USE_PREFERRED_RENDERER_BACKEND renderer_backend::opengles
#else
#error "Emscripten or iOS but no opengles"
#endif
#endif


#if !defined(PLATFORM_ANDROID)
#define PLATFORM_ANDROID TOOLCHAIN_PLATFORM_ANDROID
#endif

#if !defined(PLATFORM_EMSCRIPTEN)
#define PLATFORM_EMSCRIPTEN TOOLCHAIN_PLATFORM_EMSCRIPTEN
#endif

#if !defined(PLATFORM_IOS)
#define PLATFORM_IOS TOOLCHAIN_PLATFORM_IOS
#endif

#if !defined(PLATFORM_LINUX)
#define PLATFORM_LINUX TOOLCHAIN_PLATFORM_LINUX
#endif

#if !defined(PLATFORM_MACOS)
#define PLATFORM_MACOS TOOLCHAIN_PLATFORM_MACOS
#endif

#if !defined(PLATFORM_UNIVERSAL_WINDOWS)
#define PLATFORM_UNIVERSAL_WINDOWS TOOLCHAIN_PLATFORM_UWP
#endif

#if !defined(PLATFORM_WIN32)
#define PLATFORM_WIN32 TOOLCHAIN_PLATFORM_WIN32
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

struct ImFont;

namespace bungeegum {

constexpr renderer_backend preferred_renderer_backend = BUNGEEGUM_USE_PREFERRED_RENDERER_BACKEND;

namespace detail {

    /// @brief Instances of this struct represent cross-platform GPU renderers that will select the
    /// most appropriate graphics API depending on the platform. Macros defined above can be
    /// modified to force usage of a specific platform.
    /// @details Instances of this struct can only be moved.
    struct renderer {
    private:
        renderer() = default;

    public:
        renderer(const renderer& other) = delete;
        renderer& operator=(const renderer& other) = delete;
        renderer(renderer&& other) = default;
        renderer& operator=(renderer&& other) = default;

#if BUNGEEGUM_USE_DIRECTX
        /// @brief Creates an instance from an existing window with the DirectX 11 API.
        static renderer create_directx11(const window& existing);

        /// @brief Creates an instance from an existing window with the DirectX 12 API.
        static renderer create_directx12(const window& existing);

        /// @brief Creates an instance from an existing DirectX 11 context.
        static renderer attach_directx11(const window& existing, void*, void*);

        /// @brief Creates an instance from an existing DirectX 12 context.
        static renderer attach_directx12(const window& existing, void*, void*);
#endif

#if BUNGEEGUM_USE_OPENGL
        /// @brief Creates an instance from an existing window with the OpenGL API.
        static renderer create_opengl(const window& existing);

        /// @brief Creates an instance from an existing OpenGL context.
        static renderer attach_opengl(const window& existing);
#endif

#if BUNGEEGUM_USE_VULKAN
        /// @brief Creates an instance from an existing window with the Vulkan API.
        static renderer create_vulkan(const window& existing);
#endif

        /// @brief Begins a new frame, enabling all drawing commands.
        /// @exception Throws a backtraced exception if a new frame has already begun.
        void new_frame();

        /// @brief Ends the frame, disabling all drawing commands. Swaps the window buffers.
        /// @exception Throws a backtraced exception if the frame has already been ended.
        void present();

        /// @brief Processes ImGui events from SDL. (To be removed)
        void process_sdl_event_for_imgui(const SDL_Event* event);

        /// @brief Processes window resized events from a window, and resizes the swapchain.
        void process_window_resized_event(const window_resized_event& event);

        /// @brief Rebuilds the ImGui fonts. (To be removed)
        void rebuild_fonts();

        /// @brief Resizes the swapchain.
        void resize_swapchain(const uint2 size);

        /// @brief The color to use when clearing the screen.
        float4 clear_color = { 0.f, 0.f, 0.f, 1.f };
        
        struct renderer_data;
        std::shared_ptr<renderer_data> _data = nullptr;

    private:
        bool1 _is_rendering = false;

    };













    /// @brief 
    struct font_config {   
        
        /// @brief 
        std::size_t index = 0;

        /// @brief 
        std::size_t oversample_horizontal = 3;

        /// @brief 
        std::size_t oversample_vertical = 3;

        /// @brief 
        bool pixel_snap_horizontal = false;

        /// @brief 
        float2 glyph_extra_spacing = { 0.f, 0.f };

        /// @brief 
        float2 glyph_offset = { 0.f, 0.f };

        /// @brief 
        float rasterizer_multiply = 1.f;

        /// @brief 
        float glyph_min_advance = 0.f;
    };

    ///
    struct font_handle {
        font_handle() = default;
        font_handle(const font_handle& other) = default;
        font_handle& operator=(const font_handle& other) = default;
        font_handle(font_handle&& other) = default;
        font_handle& operator=(font_handle&& other) = default;

        // /// @brief 
        // /// @param path 
        // /// @param size 
        // /// @param config 
        // /// @param ranges 
        // void emplace(
        //     renderer& rasterizer,
        //     const std::filesystem::path& filename,
        //     const float size,
        //     const std::optional<font_config>& config = std::nullopt,
        //     unsigned short* ranges = nullptr);

        /// @brief
        /// @param rasterizer
        /// @param ttf
        /// @param count
        /// @param size
        /// @param config
        /// @param ranges
        void emplace(
            renderer& rasterizer,
            const void* ttf,
            const std::size_t count,
            const float size,
            const std::optional<font_config>& config = std::nullopt,
            unsigned short* ranges = nullptr);

        /// @brief
        /// @return
        [[nodiscard]] ImFont* get() const;

        /// @brief Gets if this instance has a value. Default created instances don't have a value
        /// until the emplace() method is called. Instances don't have a value anymore after the
        /// reset() method is called
        [[nodiscard]] bool has_value() const;

        /// @brief Resets this instance if it has a value. Default created instances don't have a
        /// value until the emplace() method is called. Instances don't have a value anymore after
        /// the reset() method is called
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
        // void emplace(
        //     renderer& rasterizer,
        //     const std::filesystem::path filename);

        /// @brief
        /// @param rasterizer
        /// @param pixels
        /// @param width
        /// @param height
        void emplace(
            const std::vector<unsigned char>& pixels,
            const std::size_t width,
            const std::size_t height);

        /// @brief
        /// @return
        [[nodiscard]] void* get() const;

        /// @brief Gets if this instance has a value. Default created instances don't have a value
        /// until the emplace() method is called. Instances don't have a value anymore after the
        /// reset() method is called
        [[nodiscard]] bool has_value() const;

        /// @brief Resets this instance if it has a value. Default created instances don't have a
        /// value until the emplace() method is called. Instances don't have a value anymore after
        /// the reset() method is called
        void reset();

    private:
        bool _has_value = false;
        Diligent::RefCntAutoPtr<Diligent::ITexture> _diligent_texture = {};
        Diligent::RefCntAutoPtr<Diligent::ITextureView> _diligent_texture_view = {};
    };
}
}