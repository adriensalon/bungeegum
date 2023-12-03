#pragma once

#include <bungeegum/core/pipeline.fwd>

namespace bungeegum {

/// @brief
enum struct renderer_backend {

    /// @brief
    directx11,

    /// @brief
    directx12,

    /// @brief
    opengl,

    /// @brief
    vulkan
};

}

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#define BUNGEEGUM_USE_PREFERRED_PIPELINE_RENDERER bungeegum::renderer_backend::opengl
#elif BUNGEEGUM_USE_DIRECTX
#define BUNGEEGUM_USE_PREFERRED_PIPELINE_RENDERER bungeegum::renderer_backend::directx11
#elif BUNGEEGUM_USE_VULKAN
#define BUNGEEGUM_USE_PREFERRED_PIPELINE_RENDERER bungeegum::renderer_backend::vulkan
#elif BUNGEEGUM_USE_OPENGL
#define BUNGEEGUM_USE_PREFERRED_PIPELINE_RENDERER bungeegum::renderer_backend::opengl
#else
#error "No graphics backend available. Please define BUNGEEGUM_USE_DIRECTX, BUNGEEGUM_USE_VULKAN or BUNGEEGUM_USE_OPENGL."
#endif

namespace bungeegum {

/// @brief
constexpr renderer_backend preferred_renderer_backend = BUNGEEGUM_USE_PREFERRED_PIPELINE_RENDERER;

template <renderer_backend backend_t>
struct pipeline_embedders;

#if BUNGEEGUM_USE_DIRECTX
template <>
struct pipeline_embedders<renderer_backend::directx11> {
    void* directx_device;
    void* directx_swapchain;
};

template <>
struct pipeline_embedders<renderer_backend::directx12> {
    void* directx_device;
    void* directx_swapchain;
};
#endif

#if BUNGEEGUM_USE_OPENGL
template <>
struct pipeline_embedders<renderer_backend::opengl> {
};
#endif

#if BUNGEEGUM_USE_VULKAN
template <>
struct pipeline_embedders<renderer_backend::vulkan> {
};
#endif

/// @brief
struct pipeline {
    pipeline() = default;
    pipeline(const pipeline& other) = delete;
    pipeline& operator=(const pipeline& other) = delete;
    pipeline(pipeline&& other) = default;
    pipeline& operator=(pipeline&& other) = default;

    /// @brief
    /// @param native_window
    pipeline& attach_window(void* native_window);

    /// @brief
    /// @tparam backend_t
    /// @param native_window
    /// @param embedders
    template <renderer_backend backend_t = preferred_renderer_backend>
    pipeline& attach_window_and_renderer(void* native_window, const pipeline_embedders<backend_t>& embedders);

    /// @brief
    /// @tparam backend_t
    template <renderer_backend backend_t = preferred_renderer_backend>
    pipeline& make_renderer();

    /// @brief
    /// @tparam backend_t
    /// @param native_window
    template <renderer_backend backend_t = preferred_renderer_backend>
    pipeline& make_window_and_renderer();

    /// @brief
    pipeline& destroy_renderer();

    /// @brief
    pipeline& destroy_window_and_renderer();

    /// @brief
    /// @return
    [[nodiscard]] bool has_renderer() const;

    /// @brief
    /// @return
    [[nodiscard]] bool has_window() const;

    /// @brief
    /// @param frames_per_second
    /// @param force_rendering
    void process_loop(const std::optional<unsigned int> frames_per_second = std::nullopt, const bool force_rendering = false);

    /// @brief Processes the frame. If force_rendering is set to true the draw pass will be
    /// executed even if no changes were requested. When set to false only the required portions
    /// of the screen will be rendered to lower battery consumption, especially on mobile devices.
    /// @param force_rendering
    pipeline& process_once(const bool force_rendering = false);

    /// @brief
    /// @param widget
    pipeline& root(const widget_id& widget);

    /// @brief
    pipeline& simulate_mouse_down(const mouse_down_interaction& interaction);

    /// @brief Sends a mouse moved interaction event
    pipeline& simulate_mouse_moved(const mouse_moved_interaction& interaction);

    /// @brief Sends a mouse moved interaction event
    pipeline& simulate_mouse_moved_relative(const mouse_moved_interaction& interaction);

    /// @brief Sends a mouse moved interaction event
    pipeline& simulate_mouse_pressed(const mouse_moved_interaction& interaction);

    /// @brief
    pipeline& simulate_mouse_up(const mouse_up_interaction& interaction);

    /// @brief Resizes the swapchain
    /// @details This has the same effect as
    pipeline& simulate_resize(const float2 size);
    // EVENTS...

    /// @brief
    pipeline& window_color(const float4 rgba);

    /// @brief
    pipeline& window_title(const std::string& description);

private:
    detail::pipeline_data _data;
    friend struct detail::pipelines_manager;
};

}