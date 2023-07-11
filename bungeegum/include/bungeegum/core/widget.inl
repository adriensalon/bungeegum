#pragma once

#include <bungeegum/backend/common.fwd>
#include <bungeegum/core/exceptions.fwd>
#include <bungeegum/core/global.fwd>

namespace bungeegum {

namespace detail {

    template <typename widget_t>
    reference_widget<widget_t> widgets_manager::create_widget(detail::value_type_t<widget_t>& reference)
    {
        return reference_widget<widget_t>(reference);
    }

    template <typename widget_t>
    std::uintptr_t widgets_manager::raw(widget_t& widget)
    {
        if constexpr (traits::is_reloadable_v<widget_t>) {
            return widget._bungeegum_object_reference;
        } else {
            return raw_cast<widget_t>(widget);
        }
    }

    template <typename widget_t>
    void widgets_manager::set_reloadable_raw(widget_t& widget, const std::uintptr_t raw_widget)
    {
        static_assert(traits::is_reloadable_v<widget_t>, "TODO");
        widget._bungeegum_object_reference = raw_widget;
    }

}

struct access {

    template <typename widget_t>
    constexpr static void detect_on_draw(reference_widget<widget_t>& widget)
    {
        if constexpr (detail::traits::has_draw_function_v<widget_t>) {
            const std::uintptr_t _raw_widget = detail::global_manager::widgets().raw(widget.get());
            detail::widget_update_data& _widget_data = detail::global_manager::widgets()[_raw_widget];
            _widget_data.drawer_command = draw_command();
            _widget_data.drawer = [widget](draw_command& command) {
                widget->draw(command);
            };
        }
    }

    template <typename widget_t>
    constexpr static void detect_on_interact(reference_widget<widget_t>& widget)
    {
        if constexpr (detail::traits::has_interact_function_v<widget_t>) {
            const std::uintptr_t _raw_widget = detail::global_manager::widgets().raw(widget.get());
            detail::widget_update_data& _widget_data = detail::global_manager::widgets()[_raw_widget];
            _widget_data.interactor_command = interact_command();
            _widget_data.interactor = [widget](interact_command& command) {
                widget->interact(command);
            };
        }
    }

    template <typename widget_t>
    constexpr static void detect_on_resolve(reference_widget<widget_t>& widget)
    {
        const std::uintptr_t _raw_widget = detail::global_manager::widgets().raw(widget.get());
        detail::widget_update_data& _widget_data = detail::global_manager::widgets()[_raw_widget];
        if constexpr (detail::traits::has_resolve_function_v<widget_t>) {
            _widget_data.resolver = [widget](resolve_command& command) {
                widget_t& okok = widget.get();
                okok.resolve(command);
            };
        } else {
            _widget_data.resolver = [&_widget_data](resolve_command& command) {
                if (_widget_data.children.empty()) {
                    command.resize(command.max_size());
                } else {
                    float2 _max_size = zero<float2>;
                    for (detail::widget_update_data& _child_widget_data : _widget_data.children) {
                        runtime_widget _child_widget = detail::global_manager::widgets().get_runtime_widget(_child_widget_data);
                        float2 _child_size = command.resolve_child(_child_widget, command.min_size(), command.max_size());
                        _max_size = glm::max(_max_size, _child_size);
                        command.position_child(_child_widget, zero<float2>);
                    }
                    command.resize(_max_size);
                }
            };
        }
    }

    template <typename widget_t>
    constexpr static void detect_on_load(reference_widget<widget_t>& widget)
    {
        const std::uintptr_t _raw_widget = detail::global_manager::widgets().raw(widget.get());
        detail::widget_update_data& _widget_data = detail::global_manager::widgets()[_raw_widget];
        _widget_data.resolver_command = resolve_command();
        if constexpr (detail::traits::has_load_function_v<widget_t>) {
            _widget_data.loader = [widget](detail::reloaded_loader& archiver) {
                archiver.load(widget);
            };
        }
    }

    template <typename widget_t>
    constexpr static void detect_on_save(reference_widget<widget_t>& widget)
    {
        const std::uintptr_t _raw_widget = detail::global_manager::widgets().raw(widget.get());
        detail::widget_update_data& _widget_data = detail::global_manager::widgets()[_raw_widget];
        _widget_data.resolver_command = resolve_command();
        if constexpr (detail::traits::has_save_function_v<widget_t>) {
            _widget_data.saver = [widget](detail::reloaded_saver& archiver) {
                archiver.save(widget);
            };
        }
    }
};

// reference_widget

template <typename widget_t>
widget_t& reference_widget<widget_t>::get() const
{
    if constexpr (detail::traits::is_reloadable_v<widget_t>) {
        return _data.get();
    } else {
        return _data;
    }
}

// template <typename widget_t>
// const widget_t& reference_widget<widget_t>::get() const
// {
//     if constexpr (detail::traits::is_reloadable_v<widget_t>) {
//         return _data.get();
//     } else {
//         return _data;
//     }
// }

template <typename widget_t>
widget_t* reference_widget<widget_t>::operator->() const
{
    return std::addressof(get());
}

// template <typename widget_t>
// const widget_t* reference_widget<widget_t>::operator->() const
// {
//     return std::addressof(get());
// }

template <typename widget_t>
reference_widget<widget_t>::reference_widget(detail::value_type_t<widget_t>& reference)
    : _data(reference)
{
}

// free

template <typename widget_t>
reference_widget<widget_t> make_reference()
{
    detail::registry_entity _entity = detail::global_manager::widgets().widgets.create_entity();

    std::uintptr_t _raw_widget;
    detail::value_type_t<widget_t>* _widget_ptr;
    if constexpr (detail::traits::is_reloadable_v<widget_t>) {
        _widget_ptr = &(detail::global_manager::widgets().widgets.create_component<detail::value_type_t<widget_t>>(
            _entity,
            detail::global_manager::backend().reload_manager->allocate<widget_t>()));
        _raw_widget = detail::raw_cast<detail::value_type_t<widget_t>>(_widget_ptr);
        detail::global_manager::widgets().set_reloadable_raw<widget_t>(_widget_ptr->get(), _raw_widget);
    } else {
        _widget_ptr = &(detail::global_manager::widgets().widgets.create_component<detail::value_type_t<widget_t>>(_entity));
        _raw_widget = detail::global_manager::widgets().raw<widget_t>(*_widget_ptr);
    }
    reference_widget<widget_t> _reference = detail::global_manager::widgets().create_widget<widget_t>(*_widget_ptr);

    detail::widget_update_data& _update_data = detail::global_manager::widgets()[_raw_widget];
    _update_data.raw_widget = _raw_widget;
    _update_data.kind = std::make_unique<std::type_index>(typeid(widget_t));
    _update_data.kind_debug = std::string(_update_data.kind->name());
    bungeegum::access::detect_on_interact(_reference);
    bungeegum::access::detect_on_resolve(_reference);
    bungeegum::access::detect_on_draw(_reference);
    bungeegum::access::detect_on_load(_reference);
    bungeegum::access::detect_on_save(_reference);

    return _reference;
}

template <typename widget_t>
widget_t& make()
{
    return make_reference<widget_t>().get();
}
}
