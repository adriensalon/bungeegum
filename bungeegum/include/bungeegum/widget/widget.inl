#pragma once

#include <bungeegum/backend/common.fwd>
#include <bungeegum/core/exceptions.fwd>
#include <bungeegum/widget/access.fwd>

namespace bungeegum {
namespace detail {

    // free

    template <typename widget_t>
    std::uintptr_t get_raw_widget(widget_t& widget)
    {
        return raw_cast(widget);
    }

    inline untyped_widget_data& get_untyped_widget(const std::uintptr_t raw_widget)
    {
        return widgets_context.registered.at(raw_widget).get();
    }

    template <typename widget_t>
    untyped_widget_data& get_untyped_widget(widget_t& widget)
    {
        std::uintptr_t _raw_widget = get_raw_widget(widget);
        return get_untyped_widget(_raw_widget);
    }

    inline bool is_widget_registered(const std::uintptr_t raw_widget)
    {
        return widgets_context.registered.find(raw_widget) != widgets_context.registered.end();
    }

    template <typename widget_t>
    bool is_widget_registered(widget_t& widget)
    {
        std::uintptr_t _raw_widget = get_raw_widget<widget_t>(widget);
        return is_widget_registered(_raw_widget);
    }

    inline bool is_widget_possessed(const std::uintptr_t raw_widget)
    {
        return widgets_context.possessed.find(raw_widget) != widgets_context.possessed.end();
    }

    template <typename widget_t>
    bool is_widget_possessed(widget_t& widget)
    {
        std::uintptr_t _raw_widget = get_raw_widget<widget_t>(widget);
        return is_widget_possessed(_raw_widget);
    }

    template <typename widget_t>
    void assign_widget(widget_t& widget, untyped_widget_data& untyped_widget, const std::uintptr_t raw_widget, const registry_entity entity)
    {
        untyped_widget.raw_widget = raw_widget;
        untyped_widget.kind = std::make_unique<std::type_index>(typeid(widget_t));
        untyped_widget.kind_debug = std::string(untyped_widget.kind->name());
        widgets_context.registered.insert_or_assign(raw_widget, std::ref(untyped_widget));
        bungeegum::access::detect_on_interact(widget);
        bungeegum::access::detect_on_resolve(widget, entity);
        bungeegum::access::detect_on_draw(widget, entity);
        // if constexpr (cereal::traits::detail::count_input_serializers<widget_t, cereal::JSONInputArchive>::value == 1) {
        //     untyped_widget.loader = [entity](reloaded_loader& archiver) {
        //         widget_t& _widget = &detail::widgets_context.widgets.get_component<reference_widget<widget_t>>(entity);
        //         archiver.load(_widget);
        //     };
        // }
        // if constexpr (cereal::traits::detail::count_output_serializers<widget_t, cereal::JSONOutputArchive>::value == 1) {
        //     untyped_widget.saver = [entity](reloaded_saver& archiver) {
        //         widget_t& _widget = &detail::widgets_context.widgets.get_component<reference_widget<widget_t>>(entity);
        //         archiver.save(_widget);
        //     };
        // }
        bungeegum::access::detect_on_load(widget, entity);
        bungeegum::access::detect_on_save(widget, entity);
    }

    template <typename widget_t>
    void register_widget(widget_t& widget, const std::uintptr_t raw_widget)
    {
        registry_entity _entity;
        if (is_widget_possessed(raw_widget)) {
            _entity = widgets_context.possessed.at(raw_widget);
            untyped_widget_data& _untyped_widget = widgets_context.widgets.get_component<untyped_widget_data>(_entity);
            assign_widget(widget, _untyped_widget, raw_widget, _entity);
            std::cout << "assign 1 \n";
        } else {
            _entity = widgets_context.widgets.create_entity();
            widgets_context.widgets.create_component<std::reference_wrapper<widget_t>>(_entity, widget);
            untyped_widget_data& _untyped_widget = widgets_context.widgets.create_component<untyped_widget_data>(_entity);
            assign_widget(widget, _untyped_widget, raw_widget, _entity);
            std::cout << "assign 2 \n";
        }
    }
}

// reference_widget

template <typename widget_t>
widget_t& reference_widget<widget_t>::get()
{
    return _data.get();
}

template <typename widget_t>
const widget_t& reference_widget<widget_t>::get() const
{
    return _data.get();
}

template <typename widget_t>
widget_t& reference_widget<widget_t>::operator&()
{
    return _data.get();
}

template <typename widget_t>
const widget_t& reference_widget<widget_t>::operator&() const
{
    return _data.get();
}

template <typename widget_t>
reference_widget<widget_t>::reference_widget(detail::reloaded<widget_t>&& reloaded)
    : _data(std::move(reloaded))
{
}

// free

template <typename widget_t, typename... widget_args_t>
reference_widget<widget_t>& make_reference(widget_args_t&&... widget_args)
{
    detail::registry_entity _entity = detail::widgets_context.widgets.create_entity();
    detail::widgets_context.widgets.create_component<detail::untyped_widget_data>(_entity);

#if 0
	widget_t& _widget = detail::widgets_context.widgets.create_component<widget_t>(_entity, std::forward<widget_args_t>(widget_args)...);
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(_widget);
#else
    reference_widget<widget_t>& _widget_reference = detail::widgets_context.widgets.create_component<reference_widget<widget_t>>(
        _entity,
        detail::reload_manager->allocate<widget_t>());
    // copy or move from args
    // _widget_reference.get() = std::move(widget_t(std::forward<widget_args_t>(widget_args)...));
    // or move
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(&_widget_reference);
#endif

    detail::widgets_context.possessed.emplace(_raw_widget, _entity);

    // std::cout << "creating widget... " << reinterpret_cast<std::uintptr_t>(&_widget_reference) << std::endl;

    // un truc comme ca
    // _widget_reference.get()._raw_widget = _raw_widget;
    return _widget_reference;
}
}
