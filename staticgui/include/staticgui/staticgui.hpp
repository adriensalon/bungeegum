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
#include <iostream>
#include <string>
#include <vector>

/// @brief
namespace staticgui {

/// @brief
/// @tparam child_widget_t
template <typename root_widget_t>
struct application {

    /// @brief
    /// @param child
    application(root_widget_t&& child);

    /// @brief
    /// @param title
    /// @return
    application window_title(const std::string& title);

    /// @brief
    /// @param width
    /// @param height
    /// @return
    application window_size(const unsigned int width, const unsigned int height);

    /// @brief
    /// @param callback
    /// @return
    application on_window_resized(std::function<void()> callback);
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
struct build_context {

    void erase() { }

    void erase_children() { }

    template <typename widget_t>
    void replace(const widget_t& widget) { }

    template <typename child_widget_t, typename widget_t>
    void replace(const child_widget_t& child_widget, const widget_t& widget) { }

    template <typename widget_t>
    void replace_children(const widget_t& widget) { }

    void maintain() { }

    template <typename child_widget_t>
    void maintain_child(const child_widget_t& child) { }

    void maintain_children() { }

    // rebuild
};

/// @brief
/// @tparam parent_widget_t
/// @tparam child_widget_t
/// @param parent
/// @param child
template <typename parent_widget_t, typename child_widget_t>
static void build_stateless(parent_widget_t* parent, const child_widget_t& child);

/// @brief
/// @tparam parent_widget_t
/// @tparam create_widget_callback_t
/// @param parent
/// @param create_widget_callback
/// @param update_widgets_callback
template <typename parent_widget_t, typename create_widget_callback_t>
static void build_stateful(
    parent_widget_t* parent,
    create_widget_callback_t create_widget_callback,
    std::function<void(build_context&)> update_widgets_callback = nullptr);

/// @brief
struct build_advanced_context : public build_context {

    // cursor etc

    template <typename root_widget_t>
    static void append(const root_widget_t& root)
    {
    }
};

/// @brief
/// @tparam parent_widget_t
/// @param parent
/// @param build_callback
/// @param maintain_callback
template <typename parent_widget_t>
static void build_advanced(
    parent_widget_t* parent,
    std::function<void(build_advanced_context&)> build_callback,
    std::function<void(build_advanced_context&)> maintain_callback = nullptr);

/// @brief
struct widget_base {

    widget_base();

private:
    unsigned long long _id;
    inline static unsigned long long _count = 0;

    template <typename parent_widget_t, typename child_widget_t>
    friend static void build_stateless(parent_widget_t*, const child_widget_t&);

    template <typename parent_widget_t, typename create_widget_callback_t>
    friend static void build_stateful(parent_widget_t*, create_widget_callback_t, std::function<void(build_context&)>);

    template <typename parent_widget_t>
    friend static void build_advanced(parent_widget_t*, std::function<void(build_advanced_context&)>, std::function<void(build_advanced_context&)>);
};

}

#include <staticgui/utils/app.hpp>
#include <staticgui/utils/draw.hpp>
#include <staticgui/utils/transform.hpp>

namespace staticgui {

template <typename root_widget_t>
application<root_widget_t>::application(root_widget_t&& child)
{
}

template <typename root_widget_t>
application<root_widget_t> application<root_widget_t>::window_title(const std::string& title)
{
    return *this;
}

template <typename root_widget_t>
application<root_widget_t> application<root_widget_t>::window_size(const unsigned int width, const unsigned int height)
{
    return *this;
}

template <typename root_widget_t>
application<root_widget_t> application<root_widget_t>::on_window_resized(std::function<void()> callback)
{
    return *this;
}

template <typename root_widget_t>
void launch(application<root_widget_t>&& app)
{
}

template <typename root_widget_t>
void attach(root_widget_t&& root)
{
}

template <typename parent_widget_t, typename child_widget_t>
static void build_stateless(parent_widget_t* parent, const child_widget_t& child)
{
}

template <typename parent_widget_t, typename create_widget_callback_t>
static void build_stateful(
    parent_widget_t* parent,
    create_widget_callback_t create_widget_callback,
    std::function<void(build_context&)> update_widgets_callback)
{
    std::cout << "parent id = " << parent->_id << std::endl;
    build_context b;
    build_context& bb(b);

    using return_widget_t = decltype(create_widget_callback(std::forward<build_context&>(bb)));

    const return_widget_t& _r = create_widget_callback(std::forward<build_context&>(bb));

    // au lieu de build stateless mettre fonction internal
    build_stateless(parent, _r);
}

template <typename parent_widget_t>
static void build_advanced(
    parent_widget_t* parent,
    std::function<void(build_advanced_context&)>
        build_callback,
    std::function<void(build_advanced_context&)> maintain_callback)
{
}

widget_base::widget_base()
{
    _id = _count;
    _count++;
}

}

#include "widgets/center.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/future_builder.hpp"
#include "widgets/gesture_detector.hpp"
#include "widgets/image.hpp"
#include "widgets/list_view.hpp"