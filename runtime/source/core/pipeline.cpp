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
#include <bungeegum/glue/hotswap.hpp>
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

    void protect_userspace(std::vector<log_data>& container, const std::function<void()>& try_callback)
    {
        protect(try_callback, [&container](const std::string& _what) {
            // uncaught error in widget ... with message ... Please use the log_error() function to detect misconfiguration etc
#if BUNGEEGUM_USE_OVERLAY
            // container.push_back(_exception);
#else
                console_log_error(_exception);
                (void)container;
#endif
        });
    }

    void save_widgets(const std::filesystem::path& archive_path, widget_update_data& root_updatable)
    {
#if BUNGEEGUM_USE_HOTSWAP
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
#if BUNGEEGUM_USE_HOTSWAP
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
        swapped_manager_data& _swapped = swapped_global();

        // pipeline_manager_data& _pipeline_manager = global().pipelines;
        // widget_manager_data& _widget_manager = global().widgets;
// #if BUNGEEGUM_USE_OVERLAY
//         _pipeline_manager.current.value().get().steps_chronometer.begin_task("resolve");
// #endif
        bool _resolve_done = false;
        std::vector<std::uintptr_t> _ids;
        while (!_resolve_done) {
            for (std::pair<const std::uintptr_t, resolve_command_data>& _it : _swapped.widgets.resolvables) {
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
// #if BUNGEEGUM_USE_OVERLAY
//                 _pipeline_manager.current.value().get().widgets_chronometer.begin_task(_updatable.clean_typename);
// #endif

                protect_userspace(_swapped.logs.userspace_errors, [&_updatable]() {
                    resolve_command_data _data = { _updatable };
                    resolve_command _command = detail::resolve_command_access::make_from_data(_data);
                    _updatable.resolver(_command);
                });
// #if BUNGEEGUM_USE_OVERLAY
//                 _pipeline_manager.current.value().get().widgets_chronometer.end_task(_updatable.clean_typename);
// #endif
            }
            for (const std::uintptr_t _id : _ids) {
                _swapped.widgets.resolvables.erase(_id);
            }
            _ids.clear();
            _resolve_done = _swapped.widgets.resolvables.empty();
        }
// #if BUNGEEGUM_USE_OVERLAY
//         _pipeline_manager.current.value().get().steps_chronometer.end_task("resolve");
// #endif
    }

    void draw_widgets(ImDrawList* imgui_drawlist)
    {
        swapped_manager_data& _swapped = swapped_global();
        bool _draw_done = false;
        std::vector<std::uintptr_t> _ids;
        const std::uintptr_t _raw_pipeline = _swapped.current; // get from func args
        while (!_draw_done) {
            for (std::pair<const std::uintptr_t, bungeegum::detail::resolve_command_data>& _it : _swapped.widgets.drawables) {
                _ids.push_back(_it.first);
                const std::uintptr_t _raw_updatable = _it.first;
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
                protect_userspace(_swapped.logs.userspace_errors, [&_updatable, _raw_updatable, imgui_drawlist, _raw_pipeline]() {
                    draw_command_data _data = { _raw_updatable, _raw_pipeline, imgui_drawlist };
                    draw_command _command = detail::draw_command_access::make_from_data(_data);
                    _updatable.drawer(_command);
                });
                // #if BUNGEEGUM_USE_OVERLAY
                //                         global().pipelines.profiler_draw_chronometer.end_task(_clean_typename);
                // #endif
                // _widget_drawer_command._data.draw(imgui_drawlist);
            };
            for (const std::uintptr_t _id : _ids) {
                _swapped.widgets.drawables.erase(_id);
            }
            _ids.clear();
            _draw_done = _swapped.widgets.drawables.empty();
        }
    }

    void process_events(detail::events_manager_data& events)
    {
        // We cleanup first so that event objects whose lifetimes have expired
        // are not iterated in the next step
        for (const std::uintptr_t _raw_event : events.updatables_to_erase) {
            events.updatables.erase(_raw_event);
        }
        events.updatables_to_erase.clear();
        for (std::pair<const uintptr_t, std::reference_wrapper<detail::event_update_data>>& _update_data : events.updatables) {
            _update_data.second.get().ticker(events);
        }
    }

    void process_animations(detail::animations_manager_data& animations, const std::chrono::milliseconds& delta_time)
    {
        // We cleanup first so that animation objects whose lifetimes have expired
        // are not iterated in the next step
        for (const std::uintptr_t _raw_event : animations.updatables_to_erase) {
            animations.updatables.erase(_raw_event);
        }
        animations.updatables_to_erase.clear();
        for (std::pair<const uintptr_t, std::reference_wrapper<detail::animation_update_data>>& _update_data : animations.updatables) {
            _update_data.second.get().ticker(delta_time);
        }
    }

    bool process_widgets(const float2 viewport_size, const std::chrono::milliseconds& delta_time, widget_update_data& root_updatable)
    {
        swapped_manager_data& _swapped = swapped_global();

        // _global.pipelines.current.value().get().steps_chronometer.begin_task("animations");
        process_animations(_swapped.animations, delta_time);
        // _global.pipelines.current.value().get().steps_chronometer.end_task("animations");

        // _global.pipelines.current.value().get().steps_chronometer.begin_task("events");
        process_events(_swapped.events);
        // _global.pipelines.current.value().get().steps_chronometer.end_task("events");

        // _global.pipelines.current.value().get().steps_chronometer.begin_task("interact pass");
        // interact_widgets();
        // _global.pipelines.current.value().get().steps_chronometer.end_task("interact pass");

        // _global.pipelines.current.value().get().steps_chronometer.begin_task("resolve pass");
        resolve_widgets(viewport_size, root_updatable);
        // _global.pipelines.current.value().get().steps_chronometer.end_task("resolve pass");

        // return (has_userspace_thrown() || !global().widgets.drawables.empty());

// #if BUNGEEGUM_USE_OVERLAY
//         _global.pipelines.current.value().get().lifetime_duration += delta_time;
// #endif

        return !_swapped.widgets.drawables.empty();
    }

    void setup_window(window_handle& pipeline_window, const pipeline_bindings& provider)
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

    void setup_renderer_directx11(pipeline_data& data, const pipeline_bindings& provider)
    {
#if !BUNGEEGUM_USE_DIRECTX
        throw backtraced_exception("");
#else
        if (provider.directx_device_ptr && provider.directx_swapchain_ptr) {
            // data.pipeline_renderer.attach_directx11(data.pipeline_window,
            //     provider.directx_device_ptr,
            //     provider.directx_swapchain_ptr);
        } else {
            data.pipeline_renderer.emplace_new_directx11(data.pipeline_window);
        }
#endif
    }

    void setup_renderer_directx12(pipeline_data& data, const pipeline_bindings& provider)
    {
#if !BUNGEEGUM_USE_DIRECTX
        throw backtraced_exception("");
#else
        if (provider.directx_device_ptr && provider.directx_swapchain_ptr) {
            // data.pipeline_renderer.attach_directx12(data.pipeline_window,
            //     provider.directx_device_ptr,
            //     provider.directx_swapchain_ptr);
        } else {
            data.pipeline_renderer.emplace_new_directx12(data.pipeline_window);
        }
#endif
    }

    void setup_renderer_opengl(pipeline_data& data, const pipeline_bindings& provider)
    {
#if !BUNGEEGUM_USE_OPENGL
        throw backtraced_exception("");
#else
        if (provider.opengl_attach_to_existing) {
            // data.pipeline_renderer.attach_opengl(data.pipeline_window);
        } else {
            data.pipeline_renderer.emplace_new_opengl(data.pipeline_window);
        }
#endif
    }

    void setup_renderer_vulkan(pipeline_data& data, const pipeline_bindings& provider)
    {
#if !BUNGEEGUM_USE_VULKAN
        throw backtraced_exception("");
#else
        // todo
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
        swapped_manager_data& _swapped = swapped_global();
        std::wstringstream _update_stream;
        reload_state _reload_result = _swapped.widgets.hotswap_reloader->update(_update_stream.rdbuf());
        std::string _update_str = narrow(_update_stream.str());
        (void)_update_str;
        // std::cout << _update_str << std::endl;
        // _global.pipelines.current.value().get().userspace_messages.push_back(std::move(_update_str));
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
        swapped_manager_data& _swapped = swapped_global();
        // _pipeline.steps_chronometer.new_frame();
        // _pipeline.widgets_chronometer.new_frame();

        if constexpr (BUNGEEGUM_USE_OVERLAY || force_rendering) {
            widget_update_data& _root_updatable = data.root_updatable.value().get();
            _swapped.widgets.drawables = { { _root_updatable.raw, std::ref(_root_updatable) } };
        }
        process_widgets(viewport_size, delta_time, data.root_updatable.value());
        if (!_swapped.widgets.drawables.empty()) {
            // _pipeline.steps_chronometer.begin_task("draw pass");
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
            // _pipeline.steps_chronometer.end_task("draw pass");
        }
    }

    void setup_pipeline(pipeline_data& data)
    {
#if BUNGEEGUM_USE_OVERLAY
        data.overlay_context.emplace(data.pipeline_renderer, nullptr);
        detail::setup_overlay(data.overlay_context);
#endif
        data.user_context.emplace(data.pipeline_renderer, nullptr);
        

        detail::swapped_manager_data& _swapped = detail::swapped_global();
        _swapped.rasterizers.insert({ data.raw, std::ref(data.user_context) });

        detail::setup_shaders(data);
        detail::global_manager_data& _global = detail::global2();
        _global.pipelines.pipelines.insert({ data.raw, std::ref(data) });
    }
}

template <>
pipeline<renderer_backend::directx11>::pipeline(const pipeline_bindings& provider)
{
    static_assert(BUNGEEGUM_USE_DIRECTX, "AAAAAAAAA");
    _data.raw = detail::raw_cast(this);
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);
    detail::setup_renderer_directx11(_data, provider);
    detail::setup_pipeline(_data);
}

template <>
pipeline<renderer_backend::directx12>::pipeline(const pipeline_bindings& provider)
{
    static_assert(BUNGEEGUM_USE_DIRECTX, "AAAAAAAAA");
    _data.raw = detail::raw_cast(this);
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);
    detail::setup_renderer_directx12(_data, provider);
    detail::setup_pipeline(_data);
}

template <>
pipeline<renderer_backend::opengl>::pipeline(const pipeline_bindings& provider)
{
    static_assert(BUNGEEGUM_USE_OPENGL, "AAAAAAAAA");
    _data.raw = detail::raw_cast(this);
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);
    detail::setup_renderer_opengl(_data, provider);
    detail::setup_pipeline(_data);
}

template <>
pipeline<renderer_backend::vulkan>::pipeline(const pipeline_bindings& provider)
{
    static_assert(BUNGEEGUM_USE_VULKAN, "AAAAAAAAA");
    _data.raw = detail::raw_cast(this);
    detail::setup_global_if_required();
    detail::setup_window(_data.pipeline_window, provider);
    detail::setup_renderer_vulkan(_data, provider);
    detail::setup_pipeline(_data);
}

template <renderer_backend backend_t>
pipeline<backend_t>& pipeline<backend_t>::color(const float4 rgba)
{
    (void)rgba;
    // _data.pipeline_window.color(rgba);
    return *this;
}
template pipeline<renderer_backend::directx11>& pipeline<renderer_backend::directx11>::color(const float4 rgba);
template pipeline<renderer_backend::directx12>& pipeline<renderer_backend::directx12>::color(const float4 rgba);
template pipeline<renderer_backend::opengl>& pipeline<renderer_backend::opengl>::color(const float4 rgba);
template pipeline<renderer_backend::vulkan>& pipeline<renderer_backend::vulkan>::color(const float4 rgba);

template <renderer_backend backend_t>
pipeline<backend_t>& pipeline<backend_t>::root(const widget_id root_id)
{
    detail::swapped_manager_data& _swapped = detail::swapped_global();
    const std::uintptr_t _raw = detail::widget_id_access::get_data(root_id);
    _data.root_updatable = _swapped.widgets.updatables.at(_raw);
    return *this;
}
template pipeline<renderer_backend::directx11>& pipeline<renderer_backend::directx11>::root(const widget_id root_id);
template pipeline<renderer_backend::directx12>& pipeline<renderer_backend::directx12>::root(const widget_id root_id);
template pipeline<renderer_backend::opengl>& pipeline<renderer_backend::opengl>::root(const widget_id root_id);
template pipeline<renderer_backend::vulkan>& pipeline<renderer_backend::vulkan>::root(const widget_id root_id);

template <renderer_backend backend_t>
void pipeline<backend_t>::run(const std::optional<unsigned int> frames_per_second, const bool force_rendering)
{
    if (!_data.root_updatable.has_value()) {
        // throw
    }
    _data.pipeline_window.update_loop(frames_per_second, [this, force_rendering](const BUNGEEGUM_USE_TIME_UNIT& delta_time) {
        detail::protect_library([this, &delta_time, force_rendering]() {

            // ICI SET LOOP RUN    
            detail::swapped_global().current = _data.raw;
            detail::swapped_global().default_shader = _data.default_shader;
            detail::swapped_global().mask_shader = _data.mask_shader;


            detail::update_input_frame(_data);
            float2 _viewport_size = _data.pipeline_window.get_size();
            detail::update_process_frame(_data, _viewport_size, delta_time, force_rendering, true);
            detail::update_hotswap_frame("C:/Users/adri/desktop/ok.json", _data.root_updatable.value());
        });
    });
}
template void pipeline<renderer_backend::directx11>::run(const std::optional<unsigned int> frames_per_second, const bool force_rendering);
template void pipeline<renderer_backend::directx12>::run(const std::optional<unsigned int> frames_per_second, const bool force_rendering);
template void pipeline<renderer_backend::opengl>::run(const std::optional<unsigned int> frames_per_second, const bool force_rendering);
template void pipeline<renderer_backend::vulkan>::run(const std::optional<unsigned int> frames_per_second, const bool force_rendering);

template <renderer_backend backend_t>
pipeline<backend_t>& pipeline<backend_t>::run_once(const bool force_rendering)
{
    if (!_data.root_updatable.has_value()) {
        // throw
    }
    _data.pipeline_window.update_once(9999u, [this, force_rendering](const BUNGEEGUM_USE_TIME_UNIT& delta_time) {
        detail::protect_library([this, &delta_time, force_rendering]() {
            
            // ICI SET LOOP RUN    
            detail::swapped_global().current = _data.raw;
            detail::swapped_global().default_shader = _data.default_shader;
            detail::swapped_global().mask_shader = _data.mask_shader;

            detail::update_input_frame(_data);
            float2 _viewport_size = _data.pipeline_window.get_size();
            detail::update_process_frame(_data, _viewport_size, delta_time, force_rendering, false);
            detail::update_hotswap_frame("C:/Users/adri/desktop/ok.json", _data.root_updatable.value());
            
        });
    });
    return *this;
}
template pipeline<renderer_backend::directx11>& pipeline<renderer_backend::directx11>::run_once(const bool force_rendering);
template pipeline<renderer_backend::directx12>& pipeline<renderer_backend::directx12>::run_once(const bool force_rendering);
template pipeline<renderer_backend::opengl>& pipeline<renderer_backend::opengl>::run_once(const bool force_rendering);
template pipeline<renderer_backend::vulkan>& pipeline<renderer_backend::vulkan>::run_once(const bool force_rendering);

template <renderer_backend backend_t>
pipeline<backend_t>& pipeline<backend_t>::title(const std::string& description)
{
#if !TOOLCHAIN_PLATFORM_EMSCRIPTEN
    _data.pipeline_window.title(description);
#endif
    return *this;
}
template pipeline<renderer_backend::directx11>& pipeline<renderer_backend::directx11>::title(const std::string& description);
template pipeline<renderer_backend::directx12>& pipeline<renderer_backend::directx12>::title(const std::string& description);
template pipeline<renderer_backend::opengl>& pipeline<renderer_backend::opengl>::title(const std::string& description);
template pipeline<renderer_backend::vulkan>& pipeline<renderer_backend::vulkan>::title(const std::string& description);


}