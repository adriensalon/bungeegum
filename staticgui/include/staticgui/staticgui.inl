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
#include <tree.hh>
#include <unordered_map>

#include <staticgui/staticgui.hpp>
#include <staticgui/utils/app.hpp>
#include <staticgui/utils/draw.hpp>
#include <staticgui/utils/id.hpp>
#include <staticgui/utils/transform.hpp>

namespace staticgui {

namespace internal {
    namespace detail {

        struct runtime_widget {

            // runtime_widget()
            // {
            //     std::cout << "COOL \n";
            // }

            // runtime_widget& operator=(runtime_widget&& other)
            // {
            //     display = std::move(other.display);
            //     data = std::move(other.data);
            //     return *this;
            // }
            // runtime_widget(runtime_widget&& other)
            // {
            //     *this = std::move(other);
            // }
            // ~runtime_widget()
            // {
            //     std::cout << "PAS COOL \n";
            // }

            std::string display;
            std::any data = nullptr;
            std::vector<id::id_int> children;
        };

        struct runtime_tree {
            inline static unsigned int all_widgets_count = 0;

            // vector
            inline static std::unordered_map<id::id_int, runtime_widget> made_widgets;
            inline static std::tree<runtime_widget> all_widgets;
            inline static std::unordered_map<id::id_int, runtime_widget*> all_ids;
        };

        struct application_launcher {
            internal::id::id_int internal_id = internal::id::id_generator::create();
            std::vector<internal::id::id_int> internal_children_ids;
        };

        inline static application app;
    }
}

template <typename widget_t>
application& launch(widget_t& widget)
{
    using namespace internal::detail;
    application_launcher _launcher;
    build(&_launcher, widget);

    // for (auto& _child_id : _launcher.internal_children_ids)
    //     std::cout << "main = " << _child_id << std::endl;

    // for (auto& _child_id : internal::detail::runtime_tree::made_widgets[3].children)
    //     std::cout << "main2 = " << _child_id << std::endl;

    // std::cout << "count = " << std::to_string(runtime_tree::built_widgets.size()) << std::endl;

    runtime_tree::all_widgets.insert(runtime_tree::all_widgets.begin(), {});
    std::function<void(std::tree<runtime_widget>::iterator, const internal::id::id_int)> ee = [&](std::tree<runtime_widget>::iterator parent_it, const internal::id::id_int parent_id) {
        // std::cout << "EE CHILD ID = " << parent_id << std::endl;
        runtime_widget& _runtime_parent_widget = runtime_tree::made_widgets[parent_id];
        parent_it = runtime_tree::all_widgets.append_child(parent_it, std::move(_runtime_parent_widget));

        for (auto& _child_id : parent_it->children) {

            // std::cout << "EMPLACING  " << parent_id << " -> " << _child_id << std::endl;
            // runtime_widget& _runtime_child_widget = runtime_tree::made_widgets[_child_id];

            // std::cout << "ee " << parent_id << " -> " << _child_id << std::endl;
            ee(parent_it, _child_id);
        }
    };
    ee(runtime_tree::all_widgets.begin(), _launcher.internal_children_ids[0]);

    internal::id::print_tree<runtime_widget>(runtime_tree::all_widgets);
    return app;
}

template <typename widget_t>
application& attach(widget_t& widget)
{
}

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    // std::cout << "BEFORE make <" << typeid(widget_t).name() << ">" << std::endl;

    using namespace internal::detail;
    std::any _any_widget = std::make_any<widget_t>(std::forward<widget_args_t>(widget_args)...);
    widget_t& _widget = std::any_cast<widget_t&>(_any_widget);
    runtime_widget& _runtime_widget = runtime_tree::made_widgets[_widget.internal_id];
    _runtime_widget.data = std::move(_any_widget);
    _runtime_widget.display = widget_t::internal_name;

    _widget = std::any_cast<widget_t&>(_runtime_widget.data);
    _runtime_widget.children = std::any_cast<widget_t&>(_runtime_widget.data).internal_children_ids;

    // std::cout << "2 make id = " << _widget.internal_id << std::endl;
    // for (auto k : _widget.internal_children_ids)
    //     std::cout << " child id = " << k;
    // std::cout << std::endl;

    return std::any_cast<widget_t&>(_runtime_widget.data);
}

template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget)
{
    // std::cout << "1 build id = " << widget->internal_id << std::endl;

    widget->internal_children_ids.emplace_back(child_widget.internal_id);

    // child_widget = runtime_tree::made_widgets

    // using namespace internal::detail;
    // internal::id::id_int _widget_id = widget->internal_id;
    // internal::id::id_int _child_id = child_widget.internal_id;
    // for (auto& k : runtime_tree::made_widgets) {
    //     if (k.get_id() == _child_id) {
    //         // runtime_widget& _runtime_child_widget = runtime_tree::made_widgets[_child_id];
    //         std::vector<std::pair<internal::id::id_int, runtime_widget>>& _runtime_children_widgets = runtime_tree::built_widgets[_widget_id];
    //         std::pair<internal::id::id_int, runtime_widget>& _emplaced_child = _runtime_children_widgets.emplace_back();
    //         _emplaced_child.first = std::move(_child_id);
    //         _emplaced_child.second = std::move(k);

    //         std::cout << "build <" << _widget_id << typeid(widget_t).name() << "> child <" << _child_id << typeid(child_widget_t).name() << ">" << std::endl;
    //     }
    // }
    // throw "horrible";
    // runtime_widget& _runtime_child_widget = runtime_tree::made_widgets[_child_id];
    // std::vector<std::pair<internal::id::id_int, runtime_widget>>& _runtime_children_widgets = runtime_tree::built_widgets[_widget_id];
    // std::pair<internal::id::id_int, runtime_widget>& _emplaced_child = _runtime_children_widgets.emplace_back();
    // _emplaced_child.first = std::move(_child_id);
    // _emplaced_child.second = std::move(_runtime_child_widget);

    // std::cout << "build <" << _widget_id << typeid(widget_t).name() << "> child <" << _child_id << typeid(child_widget_t).name() << ">" << std::endl;

    // runtime_tree::made_widgets.erase(_wrong_id);

    // move avec le parent dans build_widgets

    // _runtime_widget =

    // child_widget = std::any_cast<child_widget_t&>(
    //     _runtime_widget_t::built_widgets.insert(_runtime_widget_t::built_widgets.begin(),
    //                                         std::move(_runtime_widget_t::made_widgets.back()))
    //         ->data);

    // _runtime_widget_t::made_widgets.pop_back();
    // pas au debut mais
}

template <typename widget_t>
void build(widget_t* widget, std::function<void(context::advanced::painter&)> paint_callback)
{
    // std::cout << "1 build id = " << widget->internal_id << std::endl;

    using _runtime_widget_t = internal::detail::runtime_widget;
    // std::cout << "build <" << typeid(widget_t).name() << "> advanced" << std::endl;
    // _runtime_widget_t::made_widgets.pop_back();
    // internal::id::id_int _widget_id = internal::id::id_generator::create();
}

}

#include "widgets/center.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/future_builder.hpp"
#include "widgets/gesture_detector.hpp"
#include "widgets/image.hpp"
#include "widgets/list_view.hpp"