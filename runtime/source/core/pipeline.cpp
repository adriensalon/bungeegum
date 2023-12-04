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

#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
pipeline& pipeline::attach_native_window(void* native_window)
{
	(void)native_window;
    // setup_global_if_required();
    return *this;
}

template <>
pipeline& pipeline::attach_native_window_and_renderer<renderer_backend::directx11>(void* native_window, const pipeline_embedders<renderer_backend::directx11>& embedders)
{
	if (has_native_window()) {
        // throw
    }
	if (has_renderer()) {
        // throw
    }
	
    return *this;
}

template <>
pipeline& pipeline::attach_native_window_and_renderer<renderer_backend::directx12>(void* native_window, const pipeline_embedders<renderer_backend::directx12>& embedders)
{
	if (has_native_window()) {
        // throw
    }
	if (has_renderer()) {
        // throw
    }
	
    return *this;
}

template <>
pipeline& pipeline::attach_native_window_and_renderer<renderer_backend::opengl>(void* native_window, const pipeline_embedders<renderer_backend::opengl>& embedders)
{
	if (has_native_window()) {
        // throw
    }
	if (has_renderer()) {
        // throw
    }
	
    return *this;
}

template <>
pipeline& pipeline::attach_native_window_and_renderer<renderer_backend::vulkan>(void* native_window, const pipeline_embedders<renderer_backend::vulkan>& embedders)
{
	if (has_native_window()) {
        // throw
    }
	if (has_renderer()) {
        // throw
    }
	
    return *this;
}

bool pipeline::has_native_window() const
{

    return true;
}

pipeline& pipeline::make_native_window()
{
	if (has_native_window()) {
        // throw
    }
	if (has_renderer()) {
        // throw
    }

	_data.pipeline_window.create_native();
    return *this;
}
#endif

pipeline& pipeline::make_native_window_if_native()
{	
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
	make_native_window();
#endif
	return *this;
}

template <>
pipeline& pipeline::make_renderer<renderer_backend::directx11>()
{
#if BUNGEEGUM_USE_DIRECTX
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    if (!has_native_window()) {
        // throw
    }
#endif
    if (has_renderer()) {
        // throw
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
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    if (!has_native_window()) {
        // throw
    }
#endif
    if (has_renderer()) {
        // throw
    }
    _data.pipeline_renderer.create_opengl(_data.pipeline_window);
#if BUNGEEGUM_USE_OVERLAY
        detail::setup_overlay(); // loads fonts pr linstant ici
#endif
	_data.pipeline_renderer.rebuild_fonts();
	_data.viewport_size = _data.pipeline_window.get_size();
    return *this;
}

bool pipeline::has_renderer() const
{
	return _data.pipeline_renderer.has_renderer();
}

void pipeline::process_loop_and_reset(const std::optional<unsigned int> frames_per_second, const bool force_rendering)
{
    (void)force_rendering;
    if (!_data.raw_root.has_value()) {
        // throw
    }
	std::uintptr_t _copyable_id = _data.generated_id;	
	std::cout << "hello \n";
    _data.pipeline_window.update_loop(frames_per_second, [_copyable_id] (const BUNGEEGUM_USE_TIME_UNIT& delta_time) {
			
		detail::pipeline_data& _pipeline_data = detail::global().pipelines.updatables.at(_copyable_id).get();
		_pipeline_data.viewport_size = _pipeline_data.pipeline_window.get_size();
	
		// global().logs.protect_library([&]() {
		_pipeline_data.pipeline_window.poll();

		if (!_pipeline_data.pipeline_window.window_resized_events.empty()) {
			_pipeline_data.pipeline_renderer.resize(_pipeline_data.pipeline_window.window_resized_events.back().new_size);
		}

#if BUNGEEGUM_USE_OVERLAY
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
		_pipeline_data.pipeline_renderer.consume_emscripten_key_events(_pipeline_data.pipeline_window.get_emscripten_key_events());
		_pipeline_data.pipeline_renderer.consume_emscripten_mouse_events(_pipeline_data.pipeline_window.get_emscripten_mouse_events());
		_pipeline_data.pipeline_renderer.consume_emscripten_wheel_events(_pipeline_data.pipeline_window.get_emscripten_wheel_events());
#else
		_pipeline_data.pipeline_renderer.consume_sdl_events(_pipeline_data.pipeline_window.get_sdl_events());
#endif
#endif

		// store events in _pipeline_data ig

		detail::global().pipelines.profiler_frame_chronometer.new_frame();
		detail::global().pipelines.profiler_resolve_chronometer.new_frame();
		detail::global().pipelines.profiler_interact_chronometer.new_frame();
		detail::global().pipelines.profiler_draw_chronometer.new_frame();
		bool _has_ticked = detail::global().process.update(delta_time);
		if (_has_ticked) {
			detail::global().pipelines.profiler_frame_chronometer.begin_task("draw widgets");
			_pipeline_data.pipeline_renderer.new_frame();
			detail::global().process.render();
			_pipeline_data.pipeline_renderer.present();
			detail::global().pipelines.profiler_frame_chronometer.end_task("draw widgets");
		}
#if BUNGEEGUM_USE_HOTSWAP
		// FAIRE PAREIL AVANT / APRES FORCE UPDATE
		std::wstringstream _sstream;
		detail::reload_state _reload_result = detail::global().hotswap.reload_manager->update(_sstream.rdbuf());
		if (_reload_result == detail::reload_state::started_compiling) {
			detail::global().hotswap.save_widgets("C:/Users/adri/desktop/ok.json");
		} else if (_reload_result == detail::reload_state::performed_swap) {
			detail::global().hotswap.load_widgets("C:/Users/adri/desktop/ok.json");
		}
#endif




	});
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
	(void)description;
    // _data.pipeline_window.title(description);
    return *this;
}

}