#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    widget_id_data::operator std::uintptr_t() const
    {
        return _raw;
    }

    void widget_connector_data::draw()
    {
        if (!_is_constructed) {
            // throw
        }
        if (!_drawer_command.has_value()) {
            // throw
        }
        _drawer_method();
    }

    widget_id_data widget_connector_data::get_id()
    {
        if (!_is_constructed) {
            // throw
        }
        return _id;
    }

    resolve_command& widget_connector_data::get_resolve_command()
    {
        return _resolver_command;
    }

    void widget_connector_data::interact()
    {
        if (!_is_constructed) {
            // throw
        }
        if (!_interactor_command.has_value()) {
            // throw
        }
        _interactor_method();
    }

#if LIBUIWIDGET_USE_HOTSWAP
    void widget_connector_data::load(reloaded_loader& loader)
    {
        if (!_is_constructed) {
            // throw
        }
        if (_is_reloadable) {
        	_loader_method(loader);
        }
    }
#endif

    void widget_connector_data::resolve()
    {
        if (!_is_constructed) {
            // throw
        }
        _resolver_method();
    }

#if LIBUIWIDGET_USE_HOTSWAP
    void widget_connector_data::save(reloaded_saver& saver) const
    {
        if (!_is_constructed) {
            // throw
        }
        if (_is_reloadable) {
        	_saver_method(saver);
        }
    }
#endif

#if (LIBUIWIDGET_USE_HOTSWAP && LIBUIWIDGET_USE_OVERLAY)
    std::uintptr_t widget_connector_data::true_ptr() const
    {
        if (!_is_constructed) {
            // throw
        }
        return _true_ptr_method();
    }

    std::size_t widget_connector_data::true_sizeof() const
    {
        if (!_is_constructed) {
            // throw
        }
        return _true_sizeof_method();
    }
#endif

#if LIBUIWIDGET_USE_OVERLAY
    const std::type_info& widget_connector_data::type() const
    {
        if (!_is_constructed) {
            // throw
        }
        // return *(_widget_kind.get());
		return _widget_data.type();
    }
#endif

    // void widget_hierarchy_data::emplace_child(const std::uintptr_t child_id, widget_update_data& child_update_data)
    // {
    // 	if (!_owner.has_value()) {
    // 		// thjrow
    // 	}
    // 	std::unique_ptr<detail::widget_update_data>& _child_update_data_owner = child_update_data.hierarchy._owner.value().get();
    // 	_children_widgets.emplace(child_id, std::move(_child_update_data_owner));
    // }

    void widget_hierarchy_data::erase_child(const std::uintptr_t child_id)
    {
        _children_widgets.erase(child_id);
    }

    widget_update_data& widget_hierarchy_data::get_parent()
    {
        return _parent_widget.value().get();
    }

    bool widget_hierarchy_data::has_parent() const
    {
        return _parent_widget.has_value();
    }

	
	std::unordered_map<std::uintptr_t, std::unique_ptr<widget_update_data>>& widget_hierarchy_data::get_children()
	{
		return _children_widgets;
	}

    // void widget_hierarchy_data::register_parent(widget_update_data& parent_update_data, const std::uintptr_t child_id)
    // {
    // 	_parent_widget = std::ref(parent_update_data);
    // 	_owner = std::ref(parent_update_data.hierarchy._children_widgets.at(child_id));
    // }

    void widgets_processor_data::_process_draw(ImDrawList* imgui_drawlist, const bool force_rendering)
    {
        // 		bool _draw_done = false;
        //         while (!_draw_done) {
        //             for (const std::uintptr_t _raw_widget : global().widgets.drawables) {
        //                 widget_update_data& _widget_data = global().widgets[_raw_widget];
        //                 global().widgets.traverse(_widget_data, [imgui_drawlist](widget_update_data& _widget_data) {
        //                     // accumulate position anyway
        //                     resolve_command& _widget_resolver_command = _widget_data.resolver_command;
        //                     if (_widget_data.parent.has_value()) {
        //                         resolve_command& _parent_resolver_command = _widget_data.parent.value().get().resolver_command;
        //                         _widget_resolver_command._data.accumulated_position = _widget_resolver_command._data.resolved_position + _parent_resolver_command._data.accumulated_position;
        //                     }

        //                     // draw
        //                     if (_widget_data.drawer_command.has_value()) {

        //                         draw_command& _widget_drawer_command = _widget_data.drawer_command.value();
        //                         _widget_drawer_command._data.resolved_size = _widget_resolver_command._data.resolved_size;
        //                         _widget_drawer_command._data.resolved_position = _widget_resolver_command._data.accumulated_position;
        //                         _widget_drawer_command._data.commands.clear();
        // #if BUNGEEGUM_USE_OVERLAY
        //                         _widget_drawer_command._data.commands_infos.clear();
        //                         std::string _clean_typename = global().pipelines.to_clean_typename(_widget_data.kind->name());
        //                         global().pipelines.profiler_draw_chronometer.begin_task(_clean_typename);
        // #endif
        //                         global().logs.protect_userspace([&_widget_data, &_widget_drawer_command]() {
        //                             _widget_data.drawer(_widget_drawer_command);
        //                         });
        // #if BUNGEEGUM_USE_OVERLAY
        //                         global().pipelines.profiler_draw_chronometer.end_task(_clean_typename);
        // #endif
        //                         _widget_drawer_command._data.draw(imgui_drawlist);
        //                     }
        //                     return true;
        //                 });
        //                 // });
        //             };
        //             global().widgets.drawables.erase(
        //                 global().widgets.drawables.begin(),
        //                 global().widgets.drawables.end());
        //             _draw_done = global().widgets.drawables.empty();
        //         }
    }
	


    void widgets_processor_data::must_draw(std::unique_ptr<widget_update_data>& updatable)
    {

    }

    void widgets_processor_data::must_resolve(std::unique_ptr<widget_update_data>& updatable)
    {
    }

    bool widgets_processor_data::update(const widget_id_data& root_id, const float2 viewport_size, frame_events& events, const std::chrono::milliseconds& delta_time)
    {
        _process_interact();
        _process_resolve(root_id, viewport_size);
        return _drawable_weaks.empty();
    }

    void widgets_processor_data::render(const bool force_rendering)
    {
        draw_overlay([this, force_rendering](ImDrawList* imgui_drawlist) {
            _process_draw(imgui_drawlist, force_rendering);
        });
    }

    void widgets_processor_data::_process_interact()
    {
        detail::global().pipelines.profiler_frame_chronometer.begin_task("interact");
#define traverse_interact_impl(interaction_name)                                                                           \
    for (const interaction_name##_event& _event : interaction_name##_events) {                                             \
        global_widgets_manager.traverse(global_widgets_manager.root.value(), [&_event](widget_update_data& _widget_data) { \
            if (_widget_data.interactor_command.has_value()) {                                                             \
                _widget_data.interactor_command.value()._data.is_blocked = false;                                          \
                _widget_data.interactor_command.value()._data.command_data = _event;                                       \
                global().logs.protect_userspace([&_widget_data]() {                                                        \
                    _widget_data.interactor(_widget_data.interactor_command.value());                                      \
                });                                                                                                        \
                bool _retval = (!_widget_data.interactor_command.value()._data.is_blocked);                                \
                return _retval;                                                                                            \
            }                                                                                                              \
            return true;                                                                                                   \
        });                                                                                                                \
    }                                                                                                                      \
    interaction_name##_events.clear();

        // traverse_interact_impl(window_resized);
        // traverse_interact_impl(mouse_moved);
        // traverse_interact_impl(mouse_down);
        // traverse_interact_impl(mouse_up);
        // traverse_interact_impl(mouse_pressed);

#undef traverse_interact_impl
        detail::global().pipelines.profiler_frame_chronometer.end_task("interact");
    }

    void widgets_processor_data::_process_resolve(const widget_id_data& root_id, const float2 viewport_size)
    {
        detail::global().pipelines.profiler_frame_chronometer.begin_task("resolve widgets");
        bool _resolve_done = false;
        while (!_resolve_done) {
            std::size_t _iteration_size = _resolvable_weaks.size();
            for (const std::reference_wrapper<widget_update_data>& _resolvable_ref : _resolvable_weaks) {
                widget_update_data& _updatable = _resolvable_ref.get();
                resolve_command& _resolve_command = _updatable.connector.get_resolve_command();
                if (_updatable.connector.get_id() == root_id) {
                    _resolve_command._data.constraint.min_size = viewport_size;
                    _resolve_command._data.constraint.max_size = viewport_size;
                } else {
                    widget_update_data& _parent_updatable = _updatable.hierarchy.get_parent();
                    resolve_command& _parent_resolve_command = _parent_updatable.connector.get_resolve_command();
                    _resolve_command._data.constraint.min_size = _parent_resolve_command.min_size();
                    _resolve_command._data.constraint.max_size = _parent_resolve_command.max_size();
                }
#if BUNGEEGUM_USE_OVERLAY
                global().pipelines.profiler_resolve_chronometer.begin_task(_resolve_command._data.clean_typename);
#endif
                global().logs.protect_userspace([&_updatable]() {
                    _updatable.connector.resolve();
                });
#if BUNGEEGUM_USE_OVERLAY
                global().pipelines.profiler_resolve_chronometer.end_task(_resolve_command._data.clean_typename);
#endif
            };
            _resolvable_weaks.erase(
                _resolvable_weaks.begin(),
                _resolvable_weaks.begin() + _iteration_size);
            _resolve_done = _resolvable_weaks.empty();
        }
        detail::global().pipelines.profiler_frame_chronometer.end_task("resolve widgets");
    }

	bool widgets_registry_data::contains(const widget_id_data& id) const
	{
		return _all_strongs.find() != _all_strongs.end();
	}

    void widgets_registry_data::emplace(const widget_id_data& id, std::unique_ptr<widget_update_data>&& updatable)
    {		
    // _registry.toplevel_widgets.emplace(_id_data, std::move(_data_ptr));
    // _registry.indexed_references.emplace(_id_data, std::ref(_registry.toplevel_widgets[_id_data]));

    }

    std::unique_ptr<widget_update_data>& widgets_registry_data::get_updatable(const widget_id_data& id)
    {
		return _all_strongs.at(static_cast<std::uintptr_t>(id));
    }

	
	void widgets_registry_data::traverse_updatables(std::unique_ptr<widget_update_data>& root_updatable, const std::function<void(std::unique_ptr<widget_update_data>&)>& callback)
	{
		callback(root_updatable);
		for (auto& _updatable : root_updatable->hierarchy.get_children()) {
			traverse_updatables(_updatable.second, callback);
		}
	}

    // bool widgets_manager::contains(const std::uintptr_t raw_widget) const
    // {
    //     return _updatables.find(raw_widget) != _updatables.end();
    // }

    // void widgets_manager::notify_erase(const std::uintptr_t raw_widget)
    // {
    //     _updatables_to_erase.push_back(raw_widget);
    // }

    // widget_update_data& widgets_manager::operator[](const std::uintptr_t raw_widget)
    // {
    //     return _updatables[raw_widget];
    // }

    // widget_update_data& widgets_manager::operator[](const widget_id& widget)
    // {
    //     return _updatables[raw(widget)];
    // }

    // std::uintptr_t widgets_manager::raw(const widget_id& runtime_widget)
    // {
    //     return runtime_widget._data.raw_widget;
    // }

    // std::uintptr_t& widgets_manager::root()
    // {
    //     return _root;
    // }

    // std::uintptr_t widgets_manager::root() const
    // {
    //     return _root;
    // }

    // widget_update_data& widgets_manager::root_update_data()
    // {
    //     return _updatables[_root];
    // }

    // // const widget_update_data& widgets_manager::root_update_data() const
    // // {
    // //     return _updatables.insert_or_assign(_root);
    // // }

    // widget_id widgets_manager::create_runtime_widget(widget_update_data& widget_data)
    // {
    //     return widget_id(detail::widget_id_data { widget_data.raw_widget });
    // }

    // void widgets_manager::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback)
    // {
    //     std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
    //         bool _continue = iterate_callback(_update_data);
    //         if (_continue) {
    //             for (auto& _child : _update_data.children) {
    //                 _iterate(_child.get());
    //             }
    //         }
    //     };
    //     _iterate(iterate_root);
    // }

    // void widgets_manager::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback,
    //     const std::function<void(widget_update_data&)>& tail_iterate_callback)
    // {
    //     std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
    //         bool _continue = iterate_callback(_update_data);
    //         if (_continue) {
    //             for (auto& _child : _update_data.children) {
    //                 _iterate(_child.get());
    //             }
    //         }
    //         tail_iterate_callback(_update_data);
    //     };
    //     _iterate(iterate_root);
    // }
}

widget_id::widget_id(const detail::widget_id_data& data)
    : _data(data)
{
}

void destroy(const widget_id& widget)
{
    (void)widget;
    // ?
    // ?
    // ?
    // ?
    // void* _void_widget = reinterpret_cast<void*>(&widget);
    // detail::registry_entity _entity;
    // if (detail::global().widgets->.accessors.find(_void_widget) == detail::global().widgets->.accessors.end())
    //     detail::throw_error<detail::error_type::bad_implementation>("widget not found in accessors");
    // _entity = detail::global().widgets->.possessed.at(_void_widget);
    // detail::global().widgets->.widgets.destroy_entity(_entity);
    // detail::global().widgets->.possessed.erase(_void_widget);
    // detail::global().widgets->.accessors.erase(_void_widget);
}

void adopt(const widget_id& widget, const widget_id& child_widget)
{
    detail::widgets_registry_data& _registry = detail::global().widgets.registry;
    detail::widget_id_data _id = detail::widget_id_access::get_data(widget);
    detail::widget_id_data _child_id = detail::widget_id_access::get_data(child_widget);
    std::unique_ptr<detail::widget_update_data>& _update_data_ptr = _registry.get_updatable(_id);
    std::unique_ptr<detail::widget_update_data>& _child_update_data_ptr = _registry.get_updatable(_child_id);
	(void)_update_data_ptr;
	(void)_child_update_data_ptr;

    // _update_data_ptr->hierarchy._children_widgets.emplace(child_id, std::move(_child_update_data_ptr));

    // if (_child_update_data_ptr->hierarchy.has_parent()) {
    // 	_child_update_data_ptr->hierarchy.get_parent().hierarchy.erase_child(_child_id_data.id);
    // }
    // _child_update_data_ptr->hierarchy.register_parent(_id_data.reference, _child_id_data.id);
}

void abandon(const widget_id& parent_widget, const widget_id& child_widget)
{
    // detail::widget_update_data& _update_data = widget._data.reference;
    // detail::widget_update_data& _child_update_data = child_widget._data.reference;
    // std::uintptr_t _child_id = child_widget._data.id;

    // give to toplevel registry
}

bool has_parent(widget_id& widget)
{
    detail::widgets_registry_data& _registry = detail::global().widgets.registry;
    detail::widget_id_data _id = detail::widget_id_access::get_data(widget);
    detail::widget_hierarchy_data& _hierarchy = _registry.get_updatable(_id)->hierarchy;
    return _hierarchy.has_parent();
}

widget_id get_parent(const widget_id& widget)
{
    detail::widgets_registry_data& _registry = detail::global().widgets.registry;
    detail::widget_id_data _id = detail::widget_id_access::get_data(widget);
    detail::widget_hierarchy_data& _hierarchy = _registry.get_updatable(_id)->hierarchy;
    if (!_hierarchy.has_parent()) {
        // throw
    }
    return detail::widget_id_access::make_id(_hierarchy.get_parent().connector.get_id());
}

}