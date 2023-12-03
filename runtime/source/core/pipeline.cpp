#include <bungeegum/core/global.fwd>
#include <bungeegum/core/pipeline.hpp>

namespace bungeegum {
namespace detail {

    pipeline_data::pipeline_data()
    {
        setup_global_if_required(); // We only need to do this here

        generated_id = global().pipelines.generator.generate();
        global().pipelines.updatables.emplace(generated_id, std::ref(*this));
        pipeline_renderer.clear_color = float4 { 1.f, 1.f, 1.f, 1.f };
        std::uintptr_t _copyable_id = generated_id;
        pipeline_window.on_mouse_down([_copyable_id](const mouse_down_event& event) {
            pipeline_data& _data = global().pipelines.updatables.at(_copyable_id).get();
            _data.mouse_down_events.push_back(event);
        });
        pipeline_window.on_mouse_moved([_copyable_id](const mouse_moved_event& event) {
            pipeline_data& _data = global().pipelines.updatables.at(_copyable_id).get();
            _data.mouse_moved_events.push_back(event);
        });
        pipeline_window.on_mouse_pressed([_copyable_id](const mouse_pressed_event& event) {
            pipeline_data& _data = global().pipelines.updatables.at(_copyable_id).get();
            _data.mouse_pressed_events.push_back(event);
        });
        pipeline_window.on_mouse_up([_copyable_id](const mouse_up_event& event) {
            pipeline_data& _data = global().pipelines.updatables.at(_copyable_id).get();
            _data.mouse_up_events.push_back(event);
        });
        pipeline_window.on_resized([_copyable_id](const window_resized_event& event) {
            pipeline_data& _data = global().pipelines.updatables.at(_copyable_id).get();
            _data.window_resized_events.push_back(event);
        });
        pipeline_window.on_update([_copyable_id](const BUNGEEGUM_USE_TIME_UNIT& delta_time) {
            pipeline_data& _data = global().pipelines.updatables.at(_copyable_id).get();
            _data.viewport_size = _data.pipeline_window.get_size();
			
            // global().logs.protect_library([&]() {
            bool _has_polled = _data.pipeline_window.poll();
            (void)_has_polled;
            global().pipelines.profiler_frame_chronometer.new_frame();
            global().pipelines.profiler_resolve_chronometer.new_frame();
            global().pipelines.profiler_interact_chronometer.new_frame();
            global().pipelines.profiler_draw_chronometer.new_frame();
            bool _has_ticked = global().process.update(delta_time);
            if (_has_ticked) {
                global().pipelines.profiler_frame_chronometer.begin_task("draw widgets");
                _data.pipeline_renderer.new_frame();
                global().process.render();
                _data.pipeline_renderer.present();
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
    }

    pipeline_data::pipeline_data(pipeline_data&& other)
    {
        *this = std::move(other);
    }

    pipeline_data& pipeline_data::operator=(pipeline_data&& other)
    {
		generated_id = std::move(other.generated_id);
		pipeline_window = std::move(other.pipeline_window);
		pipeline_renderer = std::move(other.pipeline_renderer);
		raw_root = std::move(other.raw_root);
		window_resized_events = std::move(other.window_resized_events);
		mouse_moved_events = std::move(other.mouse_moved_events);
		mouse_down_events = std::move(other.mouse_down_events);
		mouse_up_events = std::move(other.mouse_up_events);
		mouse_pressed_events = std::move(other.mouse_pressed_events);
		viewport_size = std::move(other.viewport_size);
		global().pipelines.updatables.at(generated_id) = std::ref(*this);
        other.was_moved_from = true;
		return *this;
    }

    pipeline_data::~pipeline_data()
    {
        if (!was_moved_from) {
			global().pipelines.updatables.erase(generated_id);
        }
    }

    // bool pipelines_manager::contains(const std::string& name)
    // {
    //     return named_pipelines.find(name) != named_pipelines.end();
    // }

    // pipeline_data& pipelines_manager::operator[](const std::string& name)
    // {
    //     if (!contains(name)) {
    //         // throw
    //     }
    //     return named_pipelines.at(name);
    // }

    pipeline_data& pipelines_manager::operator[](pipeline& existing_pipeline)
    {
        return existing_pipeline._data;
    }

}

// pipeline_ref::pipeline_ref(detail::pipeline_data& data)
//     : _data(data)
// {
// }

pipeline& pipeline::attach_window(void* native_window)
{
	(void)native_window;
    // setup_global_if_required();
    return *this;
}

template <>
pipeline& pipeline::make_renderer<renderer_backend::directx11>()
{
#if BUNGEEGUM_USE_DIRECTX
    if (!has_window()) {
        // throw
    }
    if (has_renderer()) {
        destroy_renderer();
    }
    _data.pipeline_renderer.create_directx11(_data.pipeline_window);
#if BUNGEEGUM_USE_OVERLAY
        detail::setup_overlay(); // loads fonts pr linstant ici
#endif
	_data.pipeline_renderer.rebuild_fonts();
	_data.viewport_size = _data.pipeline_window.get_size();
#endif
    return *this;
}

template <>
pipeline& pipeline::make_renderer<renderer_backend::opengl>()
{
    if (!has_window()) {
        // throw
    }
    if (has_renderer()) {
        destroy_renderer();
    }
    _data.pipeline_renderer.create_opengl(_data.pipeline_window);
#if BUNGEEGUM_USE_OVERLAY
        detail::setup_overlay(); // loads fonts pr linstant ici
#endif
	_data.pipeline_renderer.rebuild_fonts();
	_data.viewport_size = _data.pipeline_window.get_size();
    return *this;
}

template <>
pipeline& pipeline::make_window_and_renderer<renderer_backend::directx11>()
{
#if BUNGEEGUM_USE_DIRECTX
	_data.pipeline_window.create();
    _data.pipeline_renderer.create_directx11(_data.pipeline_window);
#if BUNGEEGUM_USE_OVERLAY
        detail::setup_overlay(); // loads fonts pr linstant ici
#endif
	_data.pipeline_renderer.rebuild_fonts();
	_data.viewport_size = _data.pipeline_window.get_size();
#else
	// throw error
#endif
	return *this;
}

template <>
pipeline& pipeline::make_window_and_renderer<renderer_backend::opengl>()
{
	_data.pipeline_window.create();
    _data.pipeline_renderer.create_opengl(_data.pipeline_window);
#if BUNGEEGUM_USE_OVERLAY
        detail::setup_overlay(); // loads fonts pr linstant ici
#endif
	_data.pipeline_renderer.rebuild_fonts();
	_data.viewport_size = _data.pipeline_window.get_size();
	return *this;
}

pipeline& pipeline::destroy_renderer()
{
    // _data.value.pipeline_renderer.destroy();
    return *this;
}

pipeline& pipeline::destroy_window_and_renderer()
{
    // _data.value.pipeline_window.destroy();
    return *this;
}

bool pipeline::has_renderer() const
{
    // return _data.value.pipeline_renderer.is
    return true;
}

bool pipeline::has_window() const
{

    return true;
}

// template <>
// pipeline_ref make_pipeline_ref<renderer_backend::directx11>(const std::string& name)
// {
// 	detail::setup_global_if_required();
//     return detail::global().pipelines.create_directx11(name);
// }

// template <>
// pipeline_ref make_pipeline_ref<renderer_backend::directx12>(const std::string& name)
// {
// 	detail::setup_global_if_required();
//     return detail::global().pipelines.create_directx12(name);
// }

// template <>
// pipeline_ref make_pipeline_ref<renderer_backend::opengl>(const std::string& name)
// {
// 	detail::setup_global_if_required();
//     return detail::global().pipelines.create_opengl(name);
// }

// template <>
// pipeline_ref make_pipeline_ref<renderer_backend::vulkan>(const std::string& name)
// {
// 	detail::setup_global_if_required();
//     return detail::global().pipelines.create_vulkan(name);
// }

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

void pipeline::process_loop(const std::optional<unsigned int> frames_per_second, const bool force_rendering)
{
    // std::cout << "before loop\n";
    (void)force_rendering;
    if (!_data.raw_root.has_value()) {
        // throw
    }
    // std::cout << "before loop 2\n";
    _data.pipeline_window.update_loop(frames_per_second);
}

pipeline& pipeline::process_once(const bool force_rendering)
{
    (void)force_rendering;
    if (!_data.raw_root.has_value()) {
        // throw
    }
    // _data.pipeline_window.process_once();
    return *this;
}

pipeline& pipeline::root(const widget_id& widget)
{
    // std::cout << "before root\n";
    _data.raw_root = detail::global().widgets.raw(widget);
    // _data.root_widget = detail::global().widgets.raw(widget);
    // detail::global().widgets.root() = detail::global().widgets.raw(widget);
    return *this;
}

// pipeline& pipeline::window_color(const float4 rgba)
// {
// }

pipeline& pipeline::window_title(const std::string& description)
{
    _data.pipeline_window.title(description);
    return *this;
}

}