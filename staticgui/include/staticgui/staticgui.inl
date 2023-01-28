//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>
#include <optional>
#include <thread>
#include <tree.hh>
#include <unordered_map>

#include <staticgui/staticgui.hpp>
#include <staticgui/utils/app.hpp>
#include <staticgui/utils/draw.hpp>
#include <staticgui/utils/id.hpp>
#include <staticgui/utils/transform.hpp>

namespace staticgui {

namespace internal {
    namespace impl {

        struct runtime_widget_data {
            id::id_int this_id = id::id_generator::create();
            std::vector<id::id_int> children_ids;
            std::function<void(advanced_context&)> paint_callback = nullptr;
        };

        struct runtime_widget {
            std::string display_typename;
            runtime_widget_data widget_data;
            std::any data = nullptr;
        };

        // inline static application app;
        inline static std::unordered_map<id::id_int, runtime_widget> made_widgets;
        inline static std::tree<runtime_widget> all_widgets;
    }
}

template <typename widget_t>
void launch(widget_t& widget)
{
    using namespace internal::impl;
    struct application_launcher {
        runtime_widget_data internal_data;
    };
    application_launcher _launcher;
    build(&_launcher, widget); // besoin de la struct degueu pour ca ?
    all_widgets.insert(all_widgets.begin(), {});
    std::function<void(std::tree<runtime_widget>::iterator, const internal::id::id_int)> _emplace =
        [&](std::tree<runtime_widget>::iterator parent_it, const internal::id::id_int parent_id) {
            runtime_widget& _runtime_parent_widget = made_widgets[parent_id];
            parent_it = all_widgets.append_child(parent_it, std::move(_runtime_parent_widget));
            for (auto& _child_id : parent_it->widget_data.children_ids)
                _emplace(parent_it, _child_id);
        };
    _emplace(all_widgets.begin(), _launcher.internal_data.children_ids[0]);

    while (true) {
        internal::id::print_tree<runtime_widget>(all_widgets);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

template <typename widget_t>
void attach(widget_t& widget)
{
}

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    std::cout << "making " << typeid(widget_t).name() << std::endl;
    using namespace internal::impl;
    std::any _any_widget = std::make_any<widget_t>(std::forward<widget_args_t>(widget_args)...);
    widget_t& _widget = std::any_cast<widget_t&>(_any_widget);
    runtime_widget& _runtime_widget = made_widgets[_widget.internal_data.this_id];
    _runtime_widget.data = std::move(_any_widget);
    _runtime_widget.display_typename = typeid(widget_t).name();

    _widget = std::any_cast<widget_t&>(_runtime_widget.data);
    _runtime_widget.widget_data = std::any_cast<widget_t&>(_runtime_widget.data).internal_data;

    return std::any_cast<widget_t&>(_runtime_widget.data);
}

template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget, const bool is_above_root_widgets)
{
    widget->internal_data.children_ids.emplace_back(child_widget.internal_data.this_id);
}

template <typename widget_t, typename... children_widgets_t>
void build(widget_t* widget, children_widgets_t&... children, std::function<void(advanced_context&)> paint_callback, const bool is_above_root_widgets)
{
    widget->internal_data.paint_callback = paint_callback;
}

}
