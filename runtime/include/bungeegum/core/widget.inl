#pragma once

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/hotswap.fwd>
#include <bungeegum/core/log.fwd>

namespace bungeegum {

struct access {

    template <typename widget_t>
	constexpr static std::uintptr_t get_fake_raw(widget_t& widget)
	{
		static_assert(detail::traits::is_reloadable_v<widget_t>, "ERROOORRR");
		return widget._bungeegum_object_reference;
	}

    template <typename widget_t>
    constexpr static void detect_on_draw(detail::widget_connector_data& connector_data, detail::widget_ref_data<widget_t>& ref_data)
    {
        if constexpr (detail::traits::has_draw_function_v<widget_t>) {
            bungeegum::draw_command& _draw_command = connector_data._drawer_command.emplace();
            connector_data._drawer_method = [ref_data, &_draw_command]() {
                ref_data.get().draw(_draw_command);
            };
        }
    }

    //     template <typename widget_t>
    //     constexpr static void detect_on_interact(widget_ref<widget_t>& widget)
    //     {
    //         if constexpr (detail::traits::has_interact_function_v<widget_t>) {
    //             const std::uintptr_t _raw_widget = detail::global().widgets.raw(widget.get());
    //             detail::widget_update_data& _widget_data = detail::global().widgets[_raw_widget];
    //             _widget_data.interactor_command.emplace();
    //             _widget_data.interactor = [widget](interact_command& command) {
    //                 widget->interact(command);
    //             };
    //         }
    //     }

    //     template <typename widget_t>
    //     constexpr static void detect_on_resolve(widget_ref<widget_t>& widget)
    //     {
    //         const std::uintptr_t _raw_widget = detail::global().widgets.raw(widget.get());
    //         detail::widget_update_data& _widget_data = detail::global().widgets[_raw_widget];
    //         if constexpr (detail::traits::has_resolve_function_v<widget_t>) {
    //             _widget_data.resolver = [widget](resolve_command& command) {
    //                 widget_t& okok = widget.get();
    //                 okok.resolve(command);
    //             };
    //         } else {
    //             _widget_data.resolver = [&_widget_data](resolve_command& command) {
    //                 if (_widget_data.children.empty()) {
    //                     command.resize(command.max_size());
    //                 } else {
    //                     float2 _max_size = zero<float2>;
    //                     for (detail::widget_update_data& _child_widget_data : _widget_data.children) {
    //                         widget_id _child_widget = detail::global().widgets.create_runtime_widget(_child_widget_data);
    //                         float2 _child_size = command.resolve_child(_child_widget, command.min_size(), command.max_size());
    //                         _max_size = glm::max(_max_size, _child_size);
    //                         command.position_child(_child_widget, zero<float2>);
    //                     }
    //                     command.resize(_max_size);
    //                 }
    //             };
    //         }
    //     }

    //     template <typename widget_t>
    //     constexpr static void detect_on_load(widget_ref<widget_t>& widget)
    //     {
    // #if BUNGEEGUM_USE_HOTSWAP
    //         const std::uintptr_t _raw_widget = detail::global().widgets.raw(widget.get());
    //         detail::widget_update_data& _widget_data = detail::global().widgets[_raw_widget];
    //         if constexpr (detail::traits::is_reloadable_v<widget_t>) {
    //             _widget_data.loader = [widget](detail::reloaded_loader& archiver) {
    //                 archiver.load<widget_t>(const_cast<detail::reloaded<widget_t>&>(widget._data));
    //             };
    //         }
    // #else
    //         (void)widget;
    // #endif
    //     }

    //     template <typename widget_t>
    //     constexpr static void detect_on_save(widget_ref<widget_t>& widget)
    //     {
    // #if BUNGEEGUM_USE_HOTSWAP
    //         const std::uintptr_t _raw_widget = detail::global().widgets.raw(widget.get());
    //         detail::widget_update_data& _widget_data = detail::global().widgets[_raw_widget];
    //         if constexpr (detail::traits::is_reloadable_v<widget_t>) {
    //             _widget_data.saver = [widget](detail::reloaded_saver& archiver) {
    //                 archiver.save<widget_t>(const_cast<detail::reloaded<widget_t>&>(widget._data));
    //             };
    //         }
    // #else
    //         (void)widget;
    // #endif
    //     }

    //     template <typename widget_t>
    //     constexpr static void detect_on_sizeof(widget_ref<widget_t>& widget)
    //     {
    // #if BUNGEEGUM_USE_HOTSWAP
    //         const std::uintptr_t _raw_widget = detail::global().widgets.raw(widget.get());
    //         detail::widget_update_data& _widget_data = detail::global().widgets[_raw_widget];
    //         if constexpr (detail::traits::is_reloadable_v<widget_t>) {
    //             _widget_data.true_sizeof = [widget]() {
    //                 return widget.get()._bungeegum_sizeof();
    //             };
    //         } else {
    //             _widget_data.true_sizeof = [widget]() {
    //                 return sizeof(widget_t);
    //             };
    //         }
    // #else
    //         (void)widget;
    // #endif
    //     }

    //     template <typename widget_t>
    //     constexpr static void detect_on_this(widget_ref<widget_t>& widget)
    //     {
    // #if BUNGEEGUM_USE_HOTSWAP
    //         const std::uintptr_t _raw_widget = detail::global().widgets.raw(widget.get());
    //         detail::widget_update_data& _widget_data = detail::global().widgets[_raw_widget];
    //         if constexpr (detail::traits::is_reloadable_v<widget_t>) {
    //             _widget_data.true_ptr = [widget]() {
    //                 return widget.get()._bungeegum_this();
    //             };
    //         } else {
    //             _widget_data.true_ptr = [widget]() {
    //                 return detail::raw_cast<widget_t>(widget.get());
    //             };
    //         }
    // #else
    //         (void)widget;
    // #endif
    //     }
};

namespace detail {

    // widget_id_data& widgets_access::access_widget_id(const widget_id& widget)
    // {
    // 	return widget._data;
    // }

    // template <typename widget_t>
    // widget_ref<widget_t> widgets_manager::create_reference_widget(detail::value_type_t<widget_t>& reference)
    // {
    //     return widget_ref<widget_t>(reference);
    // }

    // template <typename widget_t>
    // std::uintptr_t widgets_manager::raw(widget_t& widget)
    // {
    //     if constexpr (traits::is_reloadable_v<widget_t>) {
    //         return widget._bungeegum_object_reference;
    //     } else {
    //         return raw_cast<widget_t>(widget);
    //     }
    // }

    // template <typename widget_t>
    // void widgets_manager::set_reloadable_raw(widget_t& widget, const std::uintptr_t raw_widget)
    // {
    //     static_assert(traits::is_reloadable_v<widget_t>, "TODO");
    //     widget._bungeegum_object_reference = raw_widget;
    // }

    //
    //
    //
    //
    //
    //
    //

	template <typename widget_t>
    struct widget_ref_access {

        static widget_ref<widget_t> make_ref(const widget_ref_data<widget_t>& data)
        {
            return widget_ref<widget_t>(data);
        }

        static detail::widget_ref_data<widget_t>& get_data(widget_ref<widget_t>& ref)
        {
            return ref._data;
        }
    };

    struct widget_id_access {

        static widget_id make_id(const widget_id_data& data)
        {
            return widget_id(data);
        }

        static detail::widget_id_data get_data(const widget_id& id)
        {
            return id._data;
        }
    };

    template <typename widget_t>
    widget_id_data::widget_id_data(widget_t& widget)
    {
        if constexpr (traits::is_reloadable_v<widget_t>) {
            _raw = bungeegum::access::get_fake_raw(widget);
        } else {
            _raw = raw_cast<widget_t>(widget);
        }
    }

    template <typename widget_t>
    widget_ref_data<widget_t>::widget_ref_data(value_type_t<widget_t>& value)
        : _reference(value)
        , _id(_reference.get())
    {
    }

    template <typename widget_t>
    widget_t& widget_ref_data<widget_t>::get()
    {
        if constexpr (traits::is_reloadable_v<widget_t>) {
            return _reference.get();
        } else {
            return _reference;
        }
    }

    template <typename widget_t>
    widget_id_data widget_ref_data<widget_t>::get_id() const
    {
        return _id;
    }

    template <typename widget_t>
    widget_ref_data<widget_t> widget_connector_data::emplace()
    {
#if LIBUIWIDGET_USE_OVERLAY
        // _widget_kind.emplace(typeid(widget_t));
		// uniquement clean typename ig
#endif
#if LIBUIWIDGET_USE_HOTRELOAD
        _is_reloadable = traits::is_reloadable_v<widget_t>;
#endif
        value_type_t<widget_t>* _value_widget_ptr = nullptr;
        if constexpr (traits::is_reloadable_v<widget_t>) {
#if BUNGEEGUM_USE_HOTSWAP
            hotswap_manager_data& _hotswapper = global().hotswap;
            _value_widget_ptr = &(_widget_data.emplace<value_type_t<widget_t>>(_hotswapper.allocate<widget_t>()));
#else
            _value_widget_ptr = &(_widget_data.emplace<value_type_t<widget_t>>());
#endif
        } else {
            _value_widget_ptr = &(_widget_data.emplace<value_type_t<widget_t>>());
        }

        widget_ref_data<widget_t> _widget_ref(*_value_widget_ptr);
        _id = _widget_ref.get_id();
        bungeegum::access::detect_on_draw(*this, _widget_ref);

        return _widget_ref;
    }

    template <typename widget_t>
    widget_ref_data<widget_t> widget_connector_data::get_ref()
    {
        value_type_t<widget_t>& _value = std::any_cast<value_type_t<widget_t>&>(_widget_data);
        return widget_ref_data<widget_t>(_value);
    }
}

template <typename widget_t>
widget_t& widget_ref<widget_t>::get()
{
    return _data.get();
}

template <typename widget_t>
widget_t* widget_ref<widget_t>::operator->()
{
    return std::addressof(get());
}

template <typename widget_t>
widget_ref<widget_t>::widget_ref(const detail::widget_ref_data<widget_t>& data)
    : _data(data)
{
}

template <typename widget_t>
widget_ref<widget_t> make_ref()
{
    detail::widgets_registry_data& _registry = detail::global().widgets.registry;
    std::unique_ptr<detail::widget_update_data> _updatable_ptr = std::make_unique<detail::widget_update_data>();
    detail::widget_ref_data<widget_t> _ref = _updatable_ptr->connector.emplace<widget_t>();
    _registry.emplace(_ref.get_id(), std::move(_updatable_ptr));
    return detail::widget_ref_access<widget_t>::make_ref(_ref);
}

template <typename widget_t>
widget_t& make()
{
    return make_ref<widget_t>().get();
}

template <typename widget_t>
widget_id::widget_id(widget_t* widget)
    : _data(*widget)
{
    if (!detail::global().widgets.registry.contains(_data)) {
        // throw
    }
}

template <typename widget_t>
widget_id::widget_id(widget_t& widget)
    : _data(widget)
{
    if (!detail::global().widgets.registry.contains(_data)) {
        // throw
    }
}

template <typename widget_t>
widget_id::widget_id(widget_ref<widget_t>& widget)
    : _data(widget._data.get_id())
{
    if (!detail::global().widgets.registry.contains(_data)) {
        // throw
    }
}

template <template <typename, typename> typename container_t, typename allocator_t>
void get_children(
    const widget_id& widget,
    container_t<widget_id, allocator_t>& container)
{
    (void)widget;
    (void)container;
}

template <typename widget_t>
bool has_type(const widget_id& widget)
{
    (void)widget;
    return false;
}
}
