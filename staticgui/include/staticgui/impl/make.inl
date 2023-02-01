//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    std::cout << "making " << typeid(widget_t).name() << std::endl;
    using namespace internal::impl;

    return detail::global_widgets.make<widget_t, widget_args_t...>(std::forward<widget_args_t>(widget_args)...);

    // glue::id_integer _entity = widgets_container.create_entity();
    // widget_impl& _runtime_widget = widgets_container.create_component<widget_impl>(_entity);
    // _runtime_widget.typeindex = std::make_unique<std::type_index>(typeid(widget_t));
    // _runtime_widget.untyped = std::make_any<widget_t>(std::forward<widget_args_t>(widget_args)...);
    // // _runtime_widget.owner = std::move(_entity);

    // widget_t& _widget = std::any_cast<widget_t&>(_runtime_widget.untyped);
    // _runtime_widget.data = _widget.internal_data;
    // widgets_refs_staging_container.emplace(_widget.internal_data.this_id, _runtime_widget);

    // return _widget;
}

}