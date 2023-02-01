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

template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget, const bool is_above_root_widgets)
{
    std::cout << "building " << typeid(widget_t).name() << std::endl;

    detail::global_widgets.build(widget, nullptr, child_widget);

    // widget->internal_data.children_ids.emplace_back(child_widget.internal_data.this_id);
}

template <typename widget_t, typename... children_widgets_t>
void build_advanced(widget_t* widget, std::function<void(layout&)> paint_callback, children_widgets_t&... children)
{
    std::cout << "building " << typeid(widget_t).name() << std::endl;

    detail::global_widgets.build(
        widget, [](detail::layout_state&) {}, children...);

    // widget->internal_data.paint_callback = paint_callback;
    // glue::constexpr_foreach<children_widgets_t...>([&](auto& _child_widget) {
    //     widget->internal_data.children_ids.emplace_back(_child_widget.internal_data.this_id);
    // },
    //     children...);
}

}