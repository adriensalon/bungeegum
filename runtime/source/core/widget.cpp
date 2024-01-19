#include <bungeegum/core/log.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {
	
	widget_id widget_id_access::make_from_data(const std::uintptr_t& data)
	{
		return widget_id(data);
	}

    std::uintptr_t widget_id_access::get_data(const widget_id& id)
	{
		return id._data;
	}

    // bool widget_manager_data::contains(const std::uintptr_t raw_widget) const
    // {
    //     return _updatables.find(raw_widget) != _updatables.end();
    // }

    // void widget_manager_data::notify_erase(const std::uintptr_t raw_widget)
    // {
    //     _updatables_to_erase.push_back(raw_widget);
    // }

    // widget_update_data& widget_manager_data::operator[](const std::uintptr_t raw_widget)
    // {
    //     return _updatables[raw_widget];
    // }

    // widget_update_data& widget_manager_data::operator[](const widget_id& widget)
    // {
    //     return _updatables[raw(widget)];
    // }

    // std::uintptr_t widget_manager_data::raw(const widget_id& runtime_widget)
    // {
    //     return runtime_widget._data.raw_widget;
    // }

    // std::uintptr_t& widget_manager_data::root()
    // {
    //     return _root;
    // }

    // std::uintptr_t widget_manager_data::root() const
    // {
    //     return _root;
    // }

    // widget_update_data& widget_manager_data::root_update_data()
    // {
    //     return _updatables[_root];
    // }

    // const widget_update_data& widget_manager_data::root_update_data() const
    // {
    //     return _updatables.insert_or_assign(_root);
    // }

    // widget_id widget_manager_data::create_runtime_widget(widget_update_data& widget_data)
    // {
    //     return widget_id(detail::widget_id_data { widget_data.raw_widget });
    // }

    // void widget_manager_data::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback)
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

    // void widget_manager_data::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback,
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

void adopt(const widget_id parent_id, const widget_id child_id)
{
	detail::widget_manager_data& _manager = detail::global().widgets;
	const std::uintptr_t _parent_raw = detail::widget_id_access::get_data(parent_id);
	const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _parent_updatable = _manager.updatables[_parent_raw];
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    _child_updatable.parent = _parent_updatable;
    _parent_updatable.children.emplace_back(_child_updatable);
}

void abandon(const widget_id parent_id, const widget_id child_id)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
	const std::uintptr_t _parent_raw = detail::widget_id_access::get_data(parent_id);
	const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _parent_updatable = _manager.updatables[_parent_raw];
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    _child_updatable.parent = std::nullopt;
    for (std::size_t _index = 0; _index < _parent_updatable.children.size(); _index++) {
        detail::widget_update_data& _loop_updatable = _parent_updatable.children[_index].get();
        if (_loop_updatable.raw == _child_updatable.raw) {
            _parent_updatable.children.erase(_parent_updatable.children.begin() + _index);
            break;
        }
    }
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

widget_id get_parent(const widget_id id)
{	
	detail::widget_manager_data& _manager = detail::global().widgets;
	const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
    detail::widget_update_data& _updatable = _manager.updatables[_raw];
    if (!_updatable.parent.has_value()) {
        // log_error("Error TODO");
    }
    detail::widget_update_data& _parent_updatable = _updatable.parent.value();
    return detail::widget_id_access::make_from_data(_parent_updatable.raw);
}

bool has_parent(const widget_id id)
{
	detail::widget_manager_data& _manager = detail::global().widgets;
	const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
    detail::widget_update_data& _updatable = _manager.updatables[_raw];
    return _updatable.parent.has_value();
}

}