#include <bungeegum/core/global.fwd>
#include <bungeegum/core/pipeline.hpp>

namespace bungeegum {
namespace detail {

    bool pipelines_manager::contains(const std::string& name)
    {
        return named_pipelines.find(name) != named_pipelines.end();
    }

    pipeline_ref pipelines_manager::create_directx11(const std::string& name)
    {
#if BUNGEEGUM_USE_DIRECTX
        if (contains(name)) {
            // throw
        }
        pipeline_data& _data = named_pipelines[name];
        _data.pipeline_window.create();
        _data.pipeline_renderer.create_directx11(_data.pipeline_window);
        setup(name, _data);
        return pipeline_ref(_data);
#else
        // static_assert(false, "DirectX renderer is not available.");
#endif
    }

    pipeline_ref pipelines_manager::create_directx12(const std::string& name)
    {
#if BUNGEEGUM_USE_DIRECTX
		if (contains(name)) {
            // throw
        }
        pipeline_data& _data = named_pipelines[name];
        _data.pipeline_window.create();
        _data.pipeline_renderer.create_directx12(_data.pipeline_window);
        setup(name, _data);
        return pipeline_ref(_data);
#else
        // static_assert(false, "DirectX renderer is not available.");
#endif
    }

    pipeline_ref pipelines_manager::create_opengl(const std::string& name)
    {
#if BUNGEEGUM_USE_OPENGL
        if (contains(name)) {
            // throw
        }
        pipeline_data& _data = named_pipelines[name];
        _data.pipeline_window.create();
        _data.pipeline_renderer.create_opengl(_data.pipeline_window);
        setup(name, _data);
        return pipeline_ref(_data);
#else
        static_assert(false, "OpenGL renderer is not available.");
#endif
    }
	
    pipeline_ref pipelines_manager::create_vulkan(const std::string& name)
    {
#if BUNGEEGUM_USE_VULKAN
		if (contains(name)) {
            // throw
        }
        pipeline_data& _data = named_pipelines[name];
        _data.pipeline_window.create();
        _data.pipeline_renderer.create_vulkan(_data.pipeline_window);
        setup(name, _data);
        return pipeline_ref(_data);
#else
        // static_assert(false, "DirectX renderer is not available.");
#endif
    }

    pipeline_data& pipelines_manager::operator[](const std::string& name)
    {
        if (!contains(name)) {
            // throw
        }
        return named_pipelines.at(name);
    }

    pipeline_data& pipelines_manager::operator[](pipeline_ref& existing_pipeline)
    {
        return existing_pipeline._data;
    }

    void pipelines_manager::setup(const std::string& name, pipeline_data& data)
    {
        setup_global_if_required();

        // global().logs.protect_library([&]() {
        data.viewport_size = data.pipeline_window.get_size();

#if BUNGEEGUM_USE_HOTSWAP
        // setup_if_required();
        // TODO
#endif

#if BUNGEEGUM_USE_OVERLAY
        setup_overlay();
#endif

        data.pipeline_renderer.rebuild_fonts(); /// idk why MSVC needs this otherwise gives error C2039: '__this': is not a member of 'bungeegum::detail::global_manager'

        /// mhhh
        data.pipeline_window.on_mouse_down([](const mouse_down_event& event) {
            global().process.mouse_down_events.push_back(event);
        });
        data.pipeline_window.on_mouse_moved([](const mouse_moved_event& event) {
            global().process.mouse_moved_events.push_back(event);
        });
        data.pipeline_window.on_mouse_pressed([](const mouse_pressed_event& event) {
            global().process.mouse_pressed_events.push_back(event);
        });
        data.pipeline_window.on_mouse_up([](const mouse_up_event& event) {
            global().process.mouse_up_events.push_back(event);
        });
        data.pipeline_window.on_resized([this](const window_resized_event& event) {
            global().process.window_resized_events.push_back(event);
        });

        data.pipeline_window.on_update([name]() {
            pipeline_data& _update_pipeline_data = global().pipelines[name];

            _update_pipeline_data.viewport_size = _update_pipeline_data.pipeline_window.get_size();
            // std::cout << "OK viewport size = " << viewport_size.x << ", " << viewport_size.y << std::endl;

            std::chrono::microseconds _max_fps_period_microseconds = std::chrono::microseconds(static_cast<unsigned int>(std::floorf(1000000.f / 60.f /* MAX FPS !!!*/)));
            std::chrono::microseconds _delta_time = _update_pipeline_data.profiling_stopwatch.lap_at_least(_max_fps_period_microseconds);
            std::chrono::milliseconds _delta_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_max_fps_period_microseconds);
            bool _has_polled = _update_pipeline_data.pipeline_window.poll();
            (void)_has_polled;

            global().pipelines.profiler_frame_chronometer.new_frame();
            global().pipelines.profiler_resolve_chronometer.new_frame();
            global().pipelines.profiler_interact_chronometer.new_frame();
            global().pipelines.profiler_draw_chronometer.new_frame();

            bool _has_ticked = global().process.update(_delta_milliseconds);
            if (_has_ticked) {

                global().pipelines.profiler_frame_chronometer.begin_task("draw widgets");
                _update_pipeline_data.pipeline_renderer.new_frame();
                global().process.render();
                _update_pipeline_data.pipeline_renderer.present();
                global().pipelines.profiler_frame_chronometer.end_task("draw widgets");
            }

#if BUNGEEGUM_USE_HOTSWAP
            // FAIRE PAREIL AVANT / APRES FORCE UPDATE
            std::wstringstream _sstream;
            reload_state _reload_result = global().hotswap.reload_manager->update(_sstream.rdbuf());
            if (_reload_result == reload_state::started_compiling) {
                global().hotswap.save_widgets("C:/Users/adri/desktop/ok.json");
            } else if (_reload_result == reload_state::performed_swap) {
                global().hotswap.load_widgets("C:/Users/adri/desktop/ok.json");
            }
#endif
        });
        data.pipeline_renderer.clear_color = float4 { 1.f, 1.f, 1.f, 1.f };
        // });
    }

}

pipeline_ref::pipeline_ref(detail::pipeline_data& data)
    : _data(data)
{
}

template <>
pipeline_ref make_pipeline_ref<renderer_backend::directx11>(const std::string& name)
{
	detail::setup_global_if_required();
    return detail::global().pipelines.create_directx11(name);
}

template <>
pipeline_ref make_pipeline_ref<renderer_backend::directx12>(const std::string& name)
{
	detail::setup_global_if_required();
    return detail::global().pipelines.create_directx12(name);
}

template <>
pipeline_ref make_pipeline_ref<renderer_backend::opengl>(const std::string& name)
{
	detail::setup_global_if_required();
    return detail::global().pipelines.create_opengl(name);
}

template <>
pipeline_ref make_pipeline_ref<renderer_backend::vulkan>(const std::string& name)
{
	detail::setup_global_if_required();
    return detail::global().pipelines.create_vulkan(name);
}

// template <>
// pipeline& pipeline::create<renderer_backend::directx12>()
// {
// #if ((TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP) && BUNGEEGUM_USE_DIRECTX)
//     _data.pipeline_window.create();
//     _data.renderer_backend.create_directx12(_data.pipeline_window);
//     _data.setup();
//     return *this;
// #else
//     static_assert(false, "DirectX renderer is not available.");
// #endif
// }

// template <>
// pipeline& pipeline::create<renderer_backend::opengl>()
// {
// #if BUNGEEGUM_USE_OPENGL
//     _data.pipeline_window.create();
//     _data.renderer_backend.create_opengl(_data.pipeline_window);
//     _data.setup();
//     return *this;
// #else
//     static_assert(false, "OpenGL renderer is not available.");
// #endif
// }

// template <>
// pipeline& pipeline::create<renderer_backend::vulkan>()
// {
// #if BUNGEEGUM_USE_VULKAN
//     _data.pipeline_window.create();
//     // _data.renderer_backend = std::make_unique<detail::renderer>(detail::renderer::create_vulkan(*(_pipeline._data.pipeline_window.get())));
//     _data.setup();
//     return *this;
// #else
//     static_assert(false, "Vulkan renderer is not available.");
// #endif
// }

// template <renderer_backend backend_t>
// pipeline pipeline::attach(const pipeline_embedders<backend_t>& embedders)
// {
//     pipeline _pipeline;
//     _pipeline._data.pipeline_window = std::make_unique<window>(window::attach_native(native_window));
//     if constexpr (backend_t == renderer_backend::directx11) {
// #if ((TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP) && BUNGEEGUM_USE_DIRECTX)
//         _pipeline._data.renderer_backend = std::make_unique<renderer>(renderer::attach_directx11(
//             embedders.native_window,
//             embedders.directx_device,
//             embedders.directx_swapchain));
// #else
//         static_assert(false, "DirectX renderer is not available.");
// #endif
//     } else if constexpr (backend_t == renderer_backend::directx12) {
// #if ((TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP) && BUNGEEGUM_USE_DIRECTX)
//         _pipeline._data.renderer_backend = std::make_unique<renderer>(renderer::attach_directx12(
//             embedders.native_window,
//             embedders.directx_device,
//             embedders.directx_swapchain));
// #else
//         static_assert(false, "DirectX renderer is not available.");
// #endif
//     } else if constexpr (backend_t == renderer_backend::opengl) {
// #if BUNGEEGUM_USE_OPENGL
//         // todo
// #else
//         static_assert(false, "OpenGL renderer is not available.");
// #endif
//     } else if constexpr (backend_t == renderer_backend::vulkan) {
// #if BUNGEEGUM_USE_VULKAN
//         // todo
// #else
//         static_assert(false, "Vulkan renderer is not available.");
// #endif
//     }
//     _pipeline._data.setup();
//     return _pipeline;
// }

void pipeline_ref::process_loop(const std::optional<unsigned int> frames_per_second, const bool force_rendering)
{
    (void)frames_per_second;
    (void)force_rendering;
    if (!_data.get().raw_root.has_value()) {
        // throw
    }
    _data.get().pipeline_window.update_loop();
}

pipeline_ref& pipeline_ref::process_once(const bool force_rendering)
{
    (void)force_rendering;
    if (!_data.get().raw_root.has_value()) {
        // throw
    }
    // _data.pipeline_window.process_once();
    return *this;
}

pipeline_ref& pipeline_ref::root(const widget_id& widget)
{
    _data.get().raw_root = detail::global().widgets.raw(widget);
    // _data.root_widget = detail::global().widgets.raw(widget);
    // detail::global().widgets.root() = detail::global().widgets.raw(widget);
    return *this;
}

// pipeline& pipeline::window_color(const float4 rgba)
// {
// }

pipeline_ref& pipeline_ref::window_title(const std::string& description)
{
    _data.get().pipeline_window.title(description);
    return *this;
}

}