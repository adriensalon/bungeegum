//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>
#include <initializer_list>
#include <string>
#include <vector>

namespace staticgui {

struct context {
    float get_available_width();
    float get_available_height();
};

template <typename child_widget_t>
struct application {
    application(const child_widget_t& child) { }
    application& title(const std::string& title) { return *this; }
    application& size(const unsigned int width, const unsigned int height) { return *this; }
    application& on_resized(std::function<void()> callback) { return *this; }
};

template <typename child_widget_t>
void start(const application<child_widget_t>& root_widget)
{
}

void paint_child(); // et pas de register ?

/// @brief
/// @tparam parent_widget_t
/// @tparam child_widget_t
/// @param parent
/// @param child
template <typename parent_widget_t, typename child_widget_t>
void register_child(parent_widget_t* parent, const child_widget_t& child) { }

/// @brief
/// @tparam parent_widget_t
/// @tparam children_widgets_t
/// @param parent
/// @param children
template <typename parent_widget_t, typename children_widgets_t>
void register_children(parent_widget_t* parent, const std::initializer_list<children_widgets_t> children) { }

/// @brief
/// @tparam parent_widget_t
/// @tparam ...children_widgets_t
/// @param parent
/// @param ...children
template <typename parent_widget_t, typename... children_widgets_t>
void register_children(parent_widget_t* parent, children_widgets_t&... children);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void mark_dirty(const widget_t& widget);
}

namespace staticgui {

}

// ifdef pour ca nn ?
#include "widgets/center.hpp"
#include "widgets/column.hpp"
#include "widgets/list_view.hpp"