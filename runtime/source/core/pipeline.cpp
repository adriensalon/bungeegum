#include <imgui.h>


#include <bungeegum/glue/toolchain.hpp>
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
#include <SDL.h> // event only omg
#endif

#include <iostream>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/pipeline.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/raw.hpp>
#include <bungeegum/glue/reload.hpp>
#include <bungeegum/glue/string.hpp>

namespace bungeegum {
namespace detail {

#if BUNGEEGUM_USE_OVERLAY
    extern void setup_overlay(rasterizer_handle& context);
    extern void draw_overlay();
#endif

    void protect_library(const std::function<void()>& try_callback)
    {
        protect(try_callback, [](const std::string& _what) {
            log("GALERE C UNE ERREUR DANS MON CODE qui nest pas backtracee", log_color::red);
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN

#else
                std::terminate();
#endif
        });
    }

    void protect_userspace(std::vector<backtraced_exception>& container, const std::function<void()>& try_callback)
    {
        protect(try_callback, [&container](const std::string& _what) {
            // uncaught error in widget ... with message ... Please use the log_error() function to detect misconfiguration etc
            backtraced_exception _exception(_what, 0, 0);
#if BUNGEEGUM_USE_OVERLAY
            container.push_back(_exception);
#else
                console_log_error(_exception);
                (void)container;
#endif
        });
    }

    void save_widgets(const std::filesystem::path& archive_path, widget_update_data& root_updatable)
    {
#if BUNGEEGUM_USE_HOTRELOAD
        reloaded_saver _archiver(archive_path);
        std::function<void(widget_update_data&)> _traverse_function = [&_archiver, &_traverse_function](widget_update_data& _updatable) {
            if (_updatable.saver) {
                _updatable.saver(_archiver);
            }
            for (widget_update_data& _child_updatable : _updatable.children) {
                _traverse_function(_child_updatable);
            }
        };
        _traverse_function(root_updatable);
#else
		(void)archive_path;
		(void)root_updatable;
#endif
    }

    void load_widgets(const std::filesystem::path& archive_path, widget_update_data& root_updatable)
    {
#if BUNGEEGUM_USE_HOTRELOAD
        reloaded_loader _archiver(archive_path);
        std::function<void(widget_update_data&)> _traverse_function = [&_archiver, &_traverse_function](widget_update_data& _updatable) {
            if (_updatable.loader) {
                _updatable.loader(_archiver);
            }
            for (widget_update_data& _child_updatable : _updatable.children) {
                _traverse_function(_child_updatable);
            }
        };
        _traverse_function(root_updatable);
#else
		(void)archive_path;
		(void)root_updatable;
#endif
    }

    //     void interact_widgets()
    //     {
    //         log_manager_data& _log_manager = global().logs;
    //         detail::global().pipelines.profiler_frame_chronometer.begin_task("interact");
    // #define traverse_interact_impl(interaction_name)                                                                           \
//     for (const interaction_name##_event& _event : interaction_name##_events) {                                             \
//         global_widgets_manager.traverse(global_widgets_manager.root.value(), [&_event](widget_update_data& _widget_data) { \
//             if (_widget_data.interactor_command.has_value()) {                                                             \
//                 _widget_data.interactor_command.value()._data.is_blocked = false;                                          \
//                 _widget_data.interactor_command.value()._data.command_data = _event;                                       \
//                 protect_userspace(_log_manager, [&_widget_data]() {                                                        \
//                     _widget_data.interactor(_widget_data.interactor_command.value());                                      \
//                 });                                                                                                        \
//                 bool _retval = (!_widget_data.interactor_command.value()._data.is_blocked);                                \
//                 return _retval;                                                                                            \
//             }                                                                                                              \
//             return true;                                                                                                   \
//         });                                                                                                                \
//     }                                                                                                                      \
//     interaction_name##_events.clear();
    //         // traverse_interact_impl(window_resized);
    //         // traverse_interact_impl(mouse_moved);
    //         // traverse_interact_impl(mouse_down);
    //         // traverse_interact_impl(mouse_up);
    //         // traverse_interact_impl(mouse_pressed);
    // #undef traverse_interact_impl
    //         detail::global().pipelines.profiler_frame_chronometer.end_task("interact");
    //     }

    void resolve_widgets(const float2 viewport_size, widget_update_data& root_updatable)
    {
        log_manager_data& _log_manager = global().logs;
        pipeline_manager_data& _pipeline_manager = global().pipelines;
        widget_manager_data& _widget_manager = global().widgets;
#if BUNGEEGUM_USE_OVERLAY
        _pipeline_manager.current.value().get().steps_chronometer.begin_task("resolve");
#endif
        bool _resolve_done = false;
		std::vector<std::uintptr_t> _ids;
        while (!_resolve_done) {
            for (std::pair<const std::uintptr_t, resolve_command_data>& _it : _widget_manager.resolvables) {
				_ids.push_back(_it.first);
				widget_update_data& _updatable = _it.second.get();
                if (_updatable.raw == root_updatable.raw) {
                    _updatable.local_min_size = viewport_size;
                    _updatable.local_max_size = viewport_size;
                } else {
                    widget_update_data& _parent_updatable = _updatable.parent.value().get();
                    _updatable.local_min_size = _parent_updatable.local_min_size;
                    _updatable.local_max_size = _parent_updatable.local_max_size;
                }
#if BUNGEEGUM_USE_OVERLAY
                _pipeline_manager.current.value().get().widgets_chronometer.begin_task(_updatable.clean_typename);
#endif

                protect_userspace(_log_manager.userspace_errors, [&_updatable]() {
					resolve_command_data _data = { _updatable };
                    resolve_command _command = detail::resolve_command_access::make_from_data(_data);
                    _updatable.resolver(_command);
                });
#if BUNGEEGUM_USE_OVERLAY
                _pipeline_manager.current.value().get().widgets_chronometer.end_task(_updatable.clean_typename);
#endif
            }
			for (const std::uintptr_t _id : _ids) {
				_widget_manager.resolvables.erase(_id);
			}
			_ids.clear();
			_resolve_done = _widget_manager.resolvables.empty();
        }
#if BUNGEEGUM_USE_OVERLAY
        _pipeline_manager.current.value().get().steps_chronometer.end_task("resolve");
#endif
    }

    void draw_widgets(ImDrawList* imgui_drawlist)
    {
        global_manager_data& _global = global();
        bool _draw_done = false;
		std::vector<std::uintptr_t> _ids;
        const std::uintptr_t _raw_pipeline = _global.pipelines.current.value().get().raw; // get from func args
        while (!_draw_done) {
            for (std::pair<const std::uintptr_t, bungeegum::detail::resolve_command_data>& _it : _global.widgets.drawables) {
				_ids.push_back(_it.first);
                widget_update_data& _updatable = _it.second.get();
				if (_updatable.parent.has_value()) {
                    widget_update_data& _parent_updatable = _updatable.parent.value().get();
                    _parent_updatable.local_position += _updatable.local_position;
                }
                // draw_command& _widget_drawer_command = _updatable.drawer_command;
                // _widget_drawer_command._data.resolved_size = _widget_resolver_command._data.resolved_size;
                // _widget_drawer_command._data.resolved_position = _widget_resolver_command._data.accumulated_position;
                // _widget_drawer_command._data.commands.clear();
                // #if BUNGEEGUM_USE_OVERLAY
                //                         _widget_drawer_command._data.commands_infos.clear();
                //                         std::string _clean_typename = global().pipelines.to_clean_typename(_updatable.inplace_data.type().name());
                //                         global().pipelines.profiler_draw_chronometer.begin_task(_clean_typename);
                // #endif
                protect_userspace(_global.logs.userspace_errors, [&_updatable, imgui_drawlist, _raw_pipeline]() {
                    draw_command_data _data = { _updatable, _raw_pipeline, imgui_drawlist };
                    draw_command _command = detail::draw_command_access::make_from_data(_data);
                    _updatable.drawer(_command);
                });
                // #if BUNGEEGUM_USE_OVERLAY
                //                         global().pipelines.profiler_draw_chronometer.end_task(_clean_typename);
                // #endif
                // _widget_drawer_command._data.draw(imgui_drawlist);
            };
			for (const std::uintptr_t _id : _ids) {
				_global.widgets.drawables.erase(_id);
			}
			_ids.clear();
            _draw_done = _global.widgets.drawables.empty();
        }
    }

    bool process_widgets(const float2 viewport_size, const std::chrono::milliseconds& delta_time, widget_update_data& root_updatable)
    {
        global_manager_data& _global = global();

        _global.pipelines.current.value().get().steps_chronometer.begin_task("animations");
        _global.animations.update(delta_time);
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
        _global.pipelines.current.value().get().steps_chronometer.end_task("animations");

        _global.pipelines.current.value().get().steps_chronometer.begin_task("events");
        _global.events.update();
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
        _global.pipelines.current.value().get().steps_chronometer.end_task("events");

        // interact_widgets();
        _global.pipelines.current.value().get().steps_chronometer.begin_task("resolve pass");
        resolve_widgets(viewport_size, root_updatable);
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
        _global.pipelines.current.value().get().steps_chronometer.end_task("resolve pass");

        // return (has_userspace_thrown() || !global().widgets.drawables.empty());

#if BUNGEEGUM_USE_OVERLAY
        _global.pipelines.current.value().get().lifetime_duration += delta_time;
#endif

        return !_global.widgets.drawables.empty();
    }

    void setup_window(window_handle& pipeline_window, const pipeline_provider& provider)
    {
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        pipeline_window.attach_emscripten(provider.emscripten_canvas_id);
#else
        if (provider.native_window_ptr) {
            pipeline_window.emplace_existing_native(provider.native_window_ptr);
        } else {
            pipeline_window.emplace_new_native();
        }
#endif
    }

    void setup_shaders(pipeline_data& data) 
    {
        shader_depth_descriptor _default_depth;
        _default_depth.function = Diligent::COMPARISON_FUNC_LESS;
        data.default_shader.emplace(data.user_context, shader_fragment_default(), {}, _default_depth);     
        
		shader_blend_descriptor _mask_blend;
		_mask_blend.color_mask = Diligent::COLOR_MASK::COLOR_MASK_NONE;
        shader_depth_descriptor _mask_depth;
        _mask_depth.enable_write = true;
        _mask_depth.function = Diligent::COMPARISON_FUNC_ALWAYS;
        // std::string _fragment = shader_fragment("return float4(1, 0, 0, 1) * UIW_SAMPLE(0, 0);");
        // data.mask_shader.emplace(data.user_context, _fragment, {}, _mask_depth);
        data.mask_shader.emplace(data.user_context, shader_fragment_default(), _mask_blend, _mask_depth);

#if BUNGEEGUM_USE_OVERLAY
        shader_depth_descriptor _overlay_depth;
        _overlay_depth.enable = false;
        // _overlay_depth.enable = true;
        // _overlay_depth.enable_write = false;
        // _mask_depth.function = Diligent::COMPARISON_FUNC_ALWAYS;
        data.overlay_shader.emplace(data.overlay_context, shader_fragment_default(), {}, _overlay_depth);
#endif
    }



    void update_hotswap_frame(const std::filesystem::path& serialize_path, widget_update_data& root_updatable)
    {
#if BUNGEEGUM_USE_HOTSWAP
        global_manager_data& _global = global();
        std::wstringstream _update_stream;
        reload_state _reload_result = _global.widgets.hotswap_reloader->update(_update_stream.rdbuf());
        std::string _update_str = narrow(_update_stream.str());
        _global.logs.hotswap_output.push_back(std::move(_update_str));
        if (_reload_result == reload_state::started_compiling) {
            save_widgets(serialize_path, root_updatable);
        } else if (_reload_result == reload_state::performed_swap) {
            load_widgets(serialize_path, root_updatable);
        }
#endif
    }

    void update_input_frame(pipeline_data& data)
    {
        data.pipeline_window.poll();
        if (!data.pipeline_window.window_resized_events.empty()) {
            data.pipeline_renderer.resize(data.pipeline_window.window_resized_events.back().new_size);
        }
#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
        data.user_context.consume_emscripten_key_events(data.pipeline_window.get_emscripten_key_events());
        data.user_context.consume_emscripten_mouse_events(data.pipeline_window.get_emscripten_mouse_events());
        data.user_context.consume_emscripten_wheel_events(data.pipeline_window.get_emscripten_wheel_events());
#if BUNGEEGUM_USE_OVERLAY
        data.overlay_context.consume_emscripten_key_events(data.pipeline_window.get_emscripten_key_events());
        data.overlay_context.consume_emscripten_mouse_events(data.pipeline_window.get_emscripten_mouse_events());
        data.overlay_context.consume_emscripten_wheel_events(data.pipeline_window.get_emscripten_wheel_events());
#endif
        data.pipeline_window.get_emscripten_key_events().clear();
        data.pipeline_window.get_emscripten_mouse_events().clear();
        data.pipeline_window.get_emscripten_wheel_events().clear();
#else
        data.user_context.consume_sdl_events(data.pipeline_window.get_sdl_events());
#if BUNGEEGUM_USE_OVERLAY
        data.overlay_context.consume_sdl_events(data.pipeline_window.get_sdl_events());
#endif
        data.pipeline_window.get_sdl_events().clear();
#endif
    }

    void update_process_frame(pipeline_data& data, const float2 viewport_size, const BUNGEEGUM_USE_TIME_UNIT& delta_time, const bool force_rendering, const bool exclusive_rendering)
    {
        global_manager_data& _global = global();
		pipeline_data& _pipeline = _global.pipelines.current.value().get();
        _pipeline.steps_chronometer.new_frame();
        _pipeline.widgets_chronometer.new_frame();
			
		
		if constexpr (BUNGEEGUM_USE_OVERLAY || force_rendering) {
			widget_update_data& _root_updatable = _pipeline.root_updatable.value().get();
			_global.widgets.drawables = { { _root_updatable.raw, std::ref(_root_updatable) } };
		}
		process_widgets(viewport_size, delta_time, data.root_updatable.value());
		if (!_global.widgets.drawables.empty()) {
            _pipeline.steps_chronometer.begin_task("draw pass");
            if (exclusive_rendering) {
				data.pipeline_renderer.clear_screen();
			}

            data.user_context.new_frame();
            data.user_context.use_projection_orthographic();
            data.user_context.use_shader(data.default_shader);
            ImDrawList* _drawlist = ImGui::GetBackgroundDrawList();
            draw_widgets(_drawlist);
            data.user_context.render();

#if BUNGEEGUM_USE_OVERLAY
            data.overlay_context.new_frame();
            data.overlay_context.use_projection_orthographic();
            data.overlay_context.use_shader(data.overlay_shader);
            draw_overlay();
            data.overlay_context.render();
#endif

			if (exclusive_rendering) {
            	data.pipeline_renderer.present();
			}
            _pipeline.steps_chronometer.end_task("draw pass");
        }
    }
}

#if BUNGEEGUM_USE_DIRECTX
template <>
pipeline& pipeline::setup<renderer_backend::directx11>(const pipeline_provider& provider)
{
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);
    if (provider.directx_device_ptr && provider.directx_swapchain_ptr) {
        // _data.pipeline_renderer.attach_directx11(_data.pipeline_window,
        //     provider.directx_device_ptr,
        //     provider.directx_swapchain_ptr);
    } else {
        _data.pipeline_renderer.emplace_new_directx11(_data.pipeline_window);
    }
#if BUNGEEGUM_USE_OVERLAY
    _data.overlay_context.emplace(_data.pipeline_renderer, nullptr);
    detail::setup_overlay(_data.overlay_context);
#endif
    _data.user_context.emplace(_data.pipeline_renderer, nullptr);
    detail::setup_shaders(_data);
    _data.raw = detail::raw_cast(this);
	detail::global_manager_data& _global = detail::global();
	_global.pipelines.pipelines.insert({ _data.raw, std::ref(_data) });
    return *this;
}

template <>
pipeline& pipeline::setup<renderer_backend::directx12>(const pipeline_provider& provider)
{
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);
    if (provider.directx_device_ptr && provider.directx_swapchain_ptr) {
        // _data.pipeline_renderer.attach_directx12(_data.pipeline_window,
        //     provider.directx_device_ptr,
        //     provider.directx_swapchain_ptr);
    } else {
        _data.pipeline_renderer.emplace_new_directx12(_data.pipeline_window);
    }
#if BUNGEEGUM_USE_OVERLAY
    detail::setup_overlay(_data.overlay_context);
#endif
    _data.user_context.emplace(_data.pipeline_renderer, nullptr);
    detail::setup_shaders(_data);
    _data.raw = detail::raw_cast(this);
	detail::global_manager_data& _global = detail::global();
	_global.pipelines.pipelines.insert({ _data.raw, std::ref(_data) });
    return *this;
}
#endif

template <>
pipeline& pipeline::setup<renderer_backend::opengl>(const pipeline_provider& provider)
{
#if !BUNGEEGUM_USE_OPENGL
    static_assert(false, "DirectX must be enabled");
#endif
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);
    if (provider.opengl_attach_to_existing) {
        // _data.pipeline_renderer.attach_opengl(_data.pipeline_window);
    } else {
        _data.pipeline_renderer.emplace_new_opengl(_data.pipeline_window);
    }
#if BUNGEEGUM_USE_OVERLAY
	_data.overlay_context.emplace(_data.pipeline_renderer, nullptr);
    detail::setup_overlay(_data.overlay_context);
#endif
    _data.user_context.emplace(_data.pipeline_renderer, nullptr);
    detail::setup_shaders(_data);
    _data.raw = detail::raw_cast(this);
	detail::global_manager_data& _global = detail::global();
	_global.pipelines.pipelines.insert({ _data.raw, std::ref(_data) });
    return *this;
}

template <>
pipeline& pipeline::setup<renderer_backend::vulkan>(const pipeline_provider& provider)
{
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);

    detail::setup_shaders(_data);
    _data.raw = detail::raw_cast(this);
	detail::global_manager_data& _global = detail::global();
	_global.pipelines.pipelines.insert({ _data.raw, std::ref(_data) });
    // TODO
    return *this;
}

void pipeline::run(const std::optional<unsigned int> frames_per_second, const bool force_rendering)
{
    if (!_data.root_updatable.has_value()) {
        // throw
    }
    _data.pipeline_window.update_loop(frames_per_second, [this, force_rendering](const BUNGEEGUM_USE_TIME_UNIT& delta_time) {
        detail::protect_library([this, &delta_time, force_rendering]() {
			detail::global().pipelines.current = std::ref(_data);
            detail::update_input_frame(_data);
            float2 _viewport_size = _data.pipeline_window.get_size();
            detail::update_process_frame(_data, _viewport_size, delta_time, force_rendering, true);
            detail::update_hotswap_frame("C:/Users/adri/desktop/ok.json", _data.root_updatable.value());
        });
    });
}

pipeline& pipeline::run_once(const bool force_rendering)
{
    if (!_data.root_updatable.has_value()) {
        // throw
    }
    _data.pipeline_window.update_once(9999u, [this, force_rendering](const BUNGEEGUM_USE_TIME_UNIT& delta_time) {
        detail::protect_library([this, &delta_time, force_rendering]() {			
			detail::global().pipelines.current = std::ref(_data);
            detail::update_input_frame(_data);
            float2 _viewport_size = _data.pipeline_window.get_size();
            detail::update_process_frame(_data, _viewport_size, delta_time, force_rendering, false);
            detail::update_hotswap_frame("C:/Users/adri/desktop/ok.json", _data.root_updatable.value());
        });
    });
    return *this;
}

pipeline& pipeline::root(const widget_id root_id)
{
    detail::setup_global_if_required();
    detail::global_manager_data& _global = detail::global();
    const std::uintptr_t _raw = detail::widget_id_access::get_data(root_id);
    _data.root_updatable = _global.widgets.updatables[_raw];
    return *this;
}

pipeline& pipeline::color(const float4 rgba)
{
    (void)rgba;
    // _data.pipeline_window.color(rgba);
    return *this;
}

pipeline& pipeline::title(const std::string& description)
{
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    _data.pipeline_window.title(description);
#endif
    return *this;
}

}