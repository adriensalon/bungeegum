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

/// @brief
namespace staticgui {

/// @brief
/// @tparam child_widget_t
template <typename child_widget_t>
struct application {

    /// @brief
    /// @param child
    application(const child_widget_t& child);

    /// @brief
    /// @param title
    /// @return
    application& window_title(const std::string& title);

    /// @brief
    /// @param width
    /// @param height
    /// @return
    application& window_size(const unsigned int width, const unsigned int height);

    /// @brief
    /// @param callback
    /// @return
    application& on_window_resized(std::function<void()> callback);
};

/// @brief
/// @tparam root_widget_t
/// @param app
template <typename root_widget_t>
void launch(const application<root_widget_t>& app);

/// @brief
/// @tparam root_widget_t
/// @param root
template <typename root_widget_t>
void attach(const root_widget_t& root);

/// @brief
/// @tparam root_widget_t
/// @param root
template <typename root_widget_t>
void build(const root_widget_t& root);

/// @brief
struct renderer {
    float get_available_width();
    float get_available_height();
};

}

#include <staticgui/utils/app.hpp>
#include <staticgui/utils/draw.hpp>

namespace staticgui {

template <typename child_widget_t>
application<child_widget_t>::application(const child_widget_t& child)
{
}

template <typename child_widget_t>
application<child_widget_t>& application<child_widget_t>::window_title(const std::string& title)
{
    return *this;
}

template <typename child_widget_t>
application<child_widget_t>& application<child_widget_t>::window_size(const unsigned int width, const unsigned int height)
{
    return *this;
}

template <typename child_widget_t>
application<child_widget_t>& application<child_widget_t>::on_window_resized(std::function<void()> callback)
{
    return *this;
}

template <typename root_widget_t>
void launch(const application<root_widget_t>& app)
{
}

template <typename root_widget_t>
void attach(const root_widget_t& root)
{
}

template <typename root_widget_t>
void build(const root_widget_t& root)
{
}

}

#include "widgets/center.hpp"
#include "widgets/column.hpp"
#include "widgets/image.hpp"
#include "widgets/list_view.hpp"