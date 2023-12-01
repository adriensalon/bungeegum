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
    void* native_window;
    void* directx_device;
    void* directx_swapchain;
};

template <>
struct pipeline_embedders<renderer_backend::directx12> {
    void* native_window;
    void* directx_device;
    void* directx_swapchain;
};
#endif

#if BUNGEEGUM_USE_OPENGL
template <>
struct pipeline_embedders<renderer_backend::opengl> {
    void* native_window;
};
#endif

#if BUNGEEGUM_USE_VULKAN
template <>
struct pipeline_embedders<renderer_backend::vulkan> {
    void* native_window;
};
#endif

/// @brief
/// @details
struct pipeline_ref {
    pipeline_ref() = delete;
    pipeline_ref(const pipeline_ref& other) = default;
    pipeline_ref& operator=(const pipeline_ref& other) = default;
    pipeline_ref(pipeline_ref&& other) = default;
    pipeline_ref& operator=(pipeline_ref&& other) = default;

    /// @brief
    /// @param force_rendering
    void process_loop(const std::optional<unsigned int> frames_per_second = 60u, const bool force_rendering = false);

    /// @brief Processes the frame. If force_rendering is set to true the draw pass will be
    /// executed even if no changes were requested. When set to false only the required portions
    /// of the screen will be rendered to lower battery consumption, especially on mobile devices.
    pipeline_ref& process_once(const bool force_rendering = false);

    /// @brief
    /// @param widget
    pipeline_ref& root(const widget_id& widget);

    // ROOT VIEW AND PROJECTION... (!= view et projection de chaque widget ?) peut etre juste dans les widgets ?
    // jsp + pratique ici aussi pcq pas besoin de faire un widget tampon degueu

    /// @brief
    pipeline_ref& simulate_mouse_down(const mouse_down_interaction& interaction);

    /// @brief Sends a mouse moved interaction event
    pipeline_ref& simulate_mouse_moved(const mouse_moved_interaction& interaction);

    /// @brief Sends a mouse moved interaction event
    pipeline_ref& simulate_mouse_moved_relative(const mouse_moved_interaction& interaction);

    /// @brief Sends a mouse moved interaction event
    pipeline_ref& simulate_mouse_pressed(const mouse_moved_interaction& interaction);

    /// @brief
    pipeline_ref& simulate_mouse_up(const mouse_up_interaction& interaction);

    /// @brief Resizes the swapchain
    /// @details This has the same effect as
    pipeline_ref& simulate_resize(const float2 size);
    // EVENTS...

    /// @brief
    pipeline_ref& window_color(const float4 rgba);

    /// @brief
    pipeline_ref& window_title(const std::string& description);

    // /// @brief
    // float2 viewport_size();

private:
    pipeline_ref(detail::pipeline_data& data);

    friend struct detail::pipelines_manager;
    std::reference_wrapper<detail::pipeline_data> _data;
};

/// @brief
/// @tparam backend_t
/// @param embedders
/// @return
template <renderer_backend backend_t = preferred_renderer_backend>
pipeline_ref make_pipeline_ref(const std::string& name);

/// @brief
/// @tparam backend_t
/// @param embedders
/// @return
template <renderer_backend backend_t = preferred_renderer_backend>
pipeline_ref& make_pipeline_ref_from(const std::string& name, const pipeline_embedders<backend_t>& embedders);

}