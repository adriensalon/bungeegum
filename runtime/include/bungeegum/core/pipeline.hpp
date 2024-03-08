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

/// @brief
constexpr renderer_backend preferred_renderer_backend = BUNGEEGUM_USE_PREFERRED_PIPELINE_RENDERER;

/// @brief 
struct pipeline_bindings {
	
    /// @brief 
    std::string emscripten_canvas_id = "canvas";
	
    /// @brief 
    void* native_window_ptr = nullptr;
	
    /// @brief 
    void* directx_device_ptr = nullptr;
	
    /// @brief 
    void* directx_swapchain_ptr = nullptr;
	
    /// @brief 
    bool opengl_attach_to_existing = false;
};

/// @brief
struct pipeline {
    pipeline() = default;
    pipeline(const pipeline& other) = delete;
    pipeline& operator=(const pipeline& other) = delete;
    pipeline(pipeline&& other) = default;
    pipeline& operator=(pipeline&& other) = default;
	
    /// @brief 
    /// @param backend 
    /// @param provider 
	pipeline& emplace(
        const renderer_backend backend = preferred_renderer_backend, 
        const pipeline_bindings& provider = {});

    /// @brief
    /// @param frames_per_second
    /// @param force_rendering
    void run(const std::optional<unsigned int> frames_per_second = std::nullopt, const bool force_rendering = false);

    /// @brief Processes the frame. If force_rendering is set to true the draw pass will be
    /// executed even if no changes were requested. When set to false only the required portions
    /// of the screen will be rendered to lower battery consumption, especially on mobile devices.
    /// @param force_rendering
    pipeline& run_once(const bool force_rendering = false);

    /// @brief
    /// @param id
    pipeline& root(const widget_id root_id);

    /// @brief
    pipeline& color(const float4 rgba);

    /// @brief
    pipeline& title(const std::string& description);

private:
	friend struct detail::pipeline_access;
	pipeline(const detail::pipeline_data& data);
    detail::pipeline_data _data;
};

}