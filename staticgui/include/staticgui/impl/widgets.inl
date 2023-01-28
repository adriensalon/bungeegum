//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#undef STATICGUI_WIDGET
#define STATICGUI_WIDGET(widget_t)                                        \
    staticgui::internal::impl::runtime_widget_data internal_data;         \
                                                                          \
    template <typename widget_t>                                          \
    friend void staticgui::launch(widget_t&);                             \
                                                                          \
    template <typename widget_t>                                          \
    friend void staticgui::attach(widget_t&);                             \
                                                                          \
    template <typename widget_t, typename... widget_args_t>               \
    friend widget_t& staticgui::make(widget_args_t&&...);                 \
                                                                          \
    template <typename widget_t, typename child_widget_t>                 \
    friend void staticgui::build(widget_t*, child_widget_t&, const bool); \
                                                                          \
    template <typename widget_t, typename... children_widgets_t>          \
    friend void staticgui::build(widget_t*, children_widgets_t&...,       \
        std::function<void(staticgui::advanced_context&)>, const bool);

#include <staticgui/widgets/center.hpp>
#include <staticgui/widgets/column.hpp>
#include <staticgui/widgets/container.hpp>
#include <staticgui/widgets/future_builder.hpp>
#include <staticgui/widgets/gesture_detector.hpp>
#include <staticgui/widgets/image.hpp>
#include <staticgui/widgets/list_view.hpp>