#pragma once

#include <bungeegum/core/global.fwd>

namespace bungeegum {
	
struct access {

    template <typename widget_t>
    constexpr static std::uintptr_t get_raw(widget_t& widget)
    {
        if constexpr (detail::traits::is_reloadable_v<widget_t>) {
            return detail::swapped_access::get_object_reference(widget);
        } else {
            return detail::raw_cast<widget_t>(widget);
        }
    }

    template <typename widget_t>
    constexpr static std::uintptr_t get_sizeof(widget_t& widget)
    {
        if constexpr (detail::traits::is_reloadable_v<widget_t>) {
            return detail::swapped_access::get_sizeof(widget);
        } else {
            return sizeof(widget_t);
        }
    }

    template <typename widget_t>
    constexpr static std::uintptr_t get_this(widget_t& widget)
    {
        if constexpr (detail::traits::is_reloadable_v<widget_t>) {
            return detail::swapped_access::get_this(widget);
        } else {
            return detail::raw_cast<widget_t>(widget);
        }
    }

    template <typename widget_t>
    constexpr static void invoke_draw(widget_t& widget, draw_command& command)
    {
        static_assert(detail::traits::has_draw_function_v<widget_t>);
        widget.draw(command);
    }

    template <typename widget_t>
    constexpr static void invoke_interact(widget_t& widget, interact_command& command)
    {
        static_assert(detail::traits::has_interact_function_v<widget_t>);
        widget.interact(command);
    }

    template <typename widget_t>
    constexpr static void invoke_resolve(widget_t& widget, resolve_command& command)
    {
        static_assert(detail::traits::has_resolve_function_v<widget_t>);
        widget.resolve(command);
    }
};

namespace detail {

    template <typename widget_t>
    widget_ref<widget_t> widget_ref_access<widget_t>::make_from_data(const reference_type_t<widget_t>& data)
    {
        return widget_ref<widget_t>(data);
    }

    template <typename widget_t>
    reference_type_t<widget_t>& widget_ref_access<widget_t>::get_data(widget_ref<widget_t>& ref)
    {
        return ref._data;
    }

    template <typename widget_t>
    const reference_type_t<widget_t>& widget_ref_access<widget_t>::get_data(const widget_ref<widget_t>& ref)
    {
        return ref._data;
    }

    template <typename widget_t>
    constexpr void install_on_draw(widget_ref<widget_t>& reference, detail::widget_update_data& updatable)
    {
        if constexpr (detail::traits::has_draw_function_v<widget_t>) {
            updatable.drawer = [reference](draw_command& command) {
                bungeegum::access::invoke_draw(reference.get(), command);
            };
        } else {
            updatable.drawer = [](draw_command& command) {
                command.draw_children();
            };
        }
    }

    template <typename widget_t>
    constexpr void install_on_interact(widget_ref<widget_t>& reference, detail::widget_update_data& updatable)
    {
        if constexpr (detail::traits::has_interact_function_v<widget_t>) {
            updatable.interactor = [reference](interact_command& command) {
                bungeegum::access::invoke_interact(reference.get(), command);
            };
        } else {
            updatable.interactor = [](interact_command& command) {
                command.interact_children();
            };
        }
    }

    template <typename widget_t>
    constexpr void install_on_resolve(widget_ref<widget_t>& reference, detail::widget_update_data& updatable)
    {
        if constexpr (detail::traits::has_resolve_function_v<widget_t>) {
            updatable.resolver = [reference](resolve_command& command) {
                bungeegum::access::invoke_resolve(reference.get(), command);
            };
        } else {
            updatable.resolver = [&updatable](resolve_command& command) {
                if (updatable.children.empty()) {
                    command.resize(command.get_max_size());
                } else {
                    float2 _max_size = zero<float2>;
                    for (detail::widget_update_data& _child_widget_data : updatable.children) {
                        widget_id _child_widget = detail::widget_id_access::make_from_data(_child_widget_data.raw);
                        float2 _child_size = command.resolve_child(_child_widget, command.get_min_size(), command.get_max_size());
                        _max_size = glm::max(_max_size, _child_size);
                        command.position_child(_child_widget, zero<float2>);
                    }
                    command.resize(_max_size);
                }
            };
        }
    }

    template <typename widget_t>
    constexpr void install_on_load(widget_ref<widget_t>& reference, detail::widget_update_data& updatable)
    {
#if BUNGEEGUM_USE_HOTSWAP
        if constexpr (detail::traits::is_reloadable_v<widget_t>) {
            updatable.loader = [reference](detail::swapped_load_guard& archiver) {
                archiver.load<widget_t>(widget_ref_access<widget_t>::get_data(reference));
            };
        }
#else
		(void)reference;
		(void)updatable;
#endif
    }

    template <typename widget_t>
    constexpr void install_on_save(widget_ref<widget_t>& reference, detail::widget_update_data& updatable)
    {
#if BUNGEEGUM_USE_HOTSWAP
        if constexpr (detail::traits::is_reloadable_v<widget_t>) {
            updatable.saver = [reference](detail::swapped_save_guard& archiver) {
                archiver.save<widget_t>(widget_ref_access<widget_t>::get_data(reference));
            };
        }
#else
		(void)reference;
		(void)updatable;
#endif
    }

    template <typename widget_t>
    constexpr void install_on_sizeof(widget_ref<widget_t>& reference, detail::widget_update_data& updatable)
    {
#if BUNGEEGUM_USE_HOTSWAP
        updatable.true_sizeof = [reference]() {
            return bungeegum::access::get_sizeof<widget_t>(reference.get());
        };
#else
		(void)reference;
		(void)updatable;
#endif
    }

    template <typename widget_t>
    constexpr void install_on_this(widget_ref<widget_t>& reference, detail::widget_update_data& updatable)
    {
#if BUNGEEGUM_USE_HOTSWAP
        updatable.true_ptr = [reference]() {
            return bungeegum::access::get_this<widget_t>(reference.get());
        };
#else
		(void)reference;
		(void)updatable;
#endif
    }

    template <typename widget_t>
    void install_clean_typename(detail::widget_update_data& updatable)
    {
#if BUNGEEGUM_USE_OVERLAY
        std::function<std::string(const std::string&)> _to_clean_typename = [](const std::string raw_typename) {
            std::size_t _last_space = raw_typename.find_last_of(' ');
            if (_last_space == raw_typename.npos) {
                _last_space = 0;
            }
            std::size_t _last_column = raw_typename.find_last_of(':');
            if (_last_column == raw_typename.npos) {
                _last_column = 0;
            }
            if (_last_space == 0 && _last_column == 0) {
                return raw_typename;
            }
            std::size_t _offset = std::max(_last_space, _last_column) + 1;
            std::size_t _length = raw_typename.length() - _offset;
            return raw_typename.substr(_offset, _length);
        };
        std::string _clean_typename = _to_clean_typename(typeid(widget_t).name());
        // updatable.resolver_command._data.clean_typename = _clean_typename;
        updatable.clean_typename = _clean_typename;
#endif
    }
}

template <typename widget_t>
widget_t& widget_ref<widget_t>::get() const
{
    return _data.get();
}

template <typename widget_t>
widget_t* widget_ref<widget_t>::operator->() const
{
    return std::addressof(get());
}

template <typename widget_t>
widget_ref<widget_t>::widget_ref(const detail::widget_ref_data<widget_t>& data)
    : _data(data)
{
}

template <typename widget_t>
widget_id::widget_id(widget_t* widget)
    : _data(access::get_raw(*widget))
{
    // if (!detail::global().widgets.updatables.contains(_data)) {
    //     throw detail::backtraced_exception("Errorlol");
    // }
}

template <typename widget_t>
widget_id::widget_id(widget_t& widget)
    : _data(access::get_raw(widget))
{
    // if (!detail::global().widgets.updatables.contains(_data)) {
    //     throw detail::backtraced_exception("Errorlol");
    // }
}

template <typename widget_t>
widget_id::widget_id(widget_ref<widget_t>& widget)
    : _data(access::get_raw(widget.get()))
{
    // if (!detail::global().widgets.updatables.contains(_data)) {
    //     throw detail::backtraced_exception("Errorlol");
    // }
}

template <typename widget_t>
widget_ref<widget_t> make_reference()
{
    detail::value_type_t<widget_t>* _widget_ptr;
    std::uintptr_t _raw_widget;
    std::any _inplace_data;

#if BUNGEEGUM_USE_HOTSWAP
    if constexpr (detail::traits::is_reloadable_v<widget_t>) {
        _widget_ptr = &(_inplace_data.emplace<detail::value_type_t<widget_t>>(
            detail::get_swapped_global().widgets.hotswap_reloader.allocate<widget_t>()));
        _raw_widget = detail::raw_cast<detail::value_type_t<widget_t>>(_widget_ptr);
        detail::swapped_access::get_object_reference(_widget_ptr->get()) = _raw_widget;
    } else {
        _widget_ptr = &(_inplace_data.emplace<detail::value_type_t<widget_t>>());
        _raw_widget = detail::raw_cast<detail::value_type_t<widget_t>>(_widget_ptr);
    }
#else
	_widget_ptr = &(_inplace_data.emplace<detail::value_type_t<widget_t>>());
	_raw_widget = detail::raw_cast<detail::value_type_t<widget_t>>(_widget_ptr);
#endif
    detail::widget_update_data& _updatable = detail::get_swapped_global().widgets.updatables[_raw_widget];
    _updatable.raw = _raw_widget;
    _updatable.inplace_data = std::move(_inplace_data);
    widget_ref<widget_t> _reference = detail::widget_ref_access<widget_t>::make_from_data(*_widget_ptr);
    detail::install_on_draw<widget_t>(_reference, _updatable);
    detail::install_on_interact<widget_t>(_reference, _updatable);
    detail::install_on_resolve<widget_t>(_reference, _updatable);
    detail::install_on_load<widget_t>(_reference, _updatable);
    detail::install_on_save<widget_t>(_reference, _updatable);
    detail::install_on_sizeof<widget_t>(_reference, _updatable);
    detail::install_on_this<widget_t>(_reference, _updatable);
    detail::install_clean_typename<widget_t>(_updatable);
    return _reference;
}

template <typename widget_t>
widget_t& make()
{
    return make_reference<widget_t>().get();
}

template <typename widget_t>
bool has_type(const widget_id& widget)
{
    (void)widget;
    return false;
}
}
