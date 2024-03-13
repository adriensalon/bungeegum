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
template <renderer_backend backend_t = preferred_renderer_backend>
struct pipeline {
    
    /// @brief
    pipeline() = delete;
    
    /// @brief 
    /// @param provider 
    pipeline(const pipeline_bindings& provider = {});
    
    /// @brief
    pipeline(const pipeline& other) = default;
    
    /// @brief
    pipeline& operator=(const pipeline& other) = default;
    
    /// @brief
    pipeline(pipeline&& other) = default;
    
    /// @brief
    pipeline& operator=(pipeline&& other) = default;

    /// @brief
    pipeline& color(const float4 rgba);

    /// @brief
    /// @param id
    pipeline& root(const widget_id root_id);

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
    pipeline& title(const std::string& description);

private:
	friend struct detail::pipeline_access;
    detail::pipeline_data _data;
};

/// @brief Gets a modifiable list of the preprocessor definitions for recompilation.
[[nodiscard]] std::vector<std::string>& get_hotswap_defines();

/// @brief Gets a modifiable list of the include directories for recompilation.
[[nodiscard]] std::vector<std::filesystem::path>& get_hotswap_include_directories();

/// @brief Gets a modifiable list of the source directories for recompilation.
[[nodiscard]] std::vector<std::filesystem::path>& get_hotswap_source_directories();

/// @brief Gets a modifiable list of the force compiled source files for recompilation.
[[nodiscard]] std::vector<std::filesystem::path>& get_hotswap_force_compiled_source_files();

/// @brief Gets a modifiable list of the libraries for recompilation.
[[nodiscard]] std::vector<std::filesystem::path>& get_hotswap_libraries();


}