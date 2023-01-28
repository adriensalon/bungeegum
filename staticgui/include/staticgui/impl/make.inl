//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/staticgui.hpp>

namespace staticgui {

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    std::cout << "making " << typeid(widget_t).name() << std::endl;
    using namespace internal::impl;

    internal::ecs::entity _entity(internal::impl::widgets_container);
    runtime_widget_component& _runtime_widget = _entity.create_component<runtime_widget_component>();
    _runtime_widget.typeindex = std::make_unique<std::type_index>(typeid(widget_t));
    _runtime_widget.untyped = std::make_any<widget_t>(std::forward<widget_args_t>(widget_args)...);

    widget_t& _widget = std::any_cast<widget_t&>(_runtime_widget.untyped);
    _runtime_widget.data = _widget.internal_data;
    widgets_ptrs_staging_container[_widget.internal_data.this_id] = &_runtime_widget;

    return _widget;
}

}