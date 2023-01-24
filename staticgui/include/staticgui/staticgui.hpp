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

#include <staticgui/utils/app.hpp>
#include <staticgui/utils/draw.hpp>
#include <staticgui/utils/id.hpp>
#include <staticgui/utils/transform.hpp>

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
static void build_stateless(parent_widget_t* parent, child_widget_t&& child);

/// @brief
/// @tparam parent_widget_t
/// @tparam create_widget_callback_t
/// @param parent
/// @param create_widget_callback
/// @param update_widgets_callback
template <typename parent_widget_t, typename create_widget_callback_t>
static void build_stateful(parent_widget_t* parent, create_widget_callback_t create_widget_callback, std::function<void(build_context&)> update_widgets_callback = nullptr);

/// @brief
struct build_advanced_context : public build_context {

    // cursor etc

    template <typename root_widget_t>
    static void append(const root_widget_t& root) { }
};

/// @brief
/// @tparam parent_widget_t
/// @param parent
/// @param build_callback
/// @param maintain_callback
template <typename parent_widget_t>
static void build_advanced(parent_widget_t* parent, std::function<void(build_advanced_context&)> build_callback, std::function<void(build_advanced_context&)> maintain_callback = nullptr);

/// @brief
struct base_widget : public internal::id::id_interface {

private:
    template <typename root_widget_t>
    friend struct application;

    template <typename parent_widget_t, typename child_widget_t>
    friend static void build_stateless(parent_widget_t*, child_widget_t&&);

    template <typename parent_widget_t, typename create_widget_callback_t>
    friend static void build_stateful(parent_widget_t*, create_widget_callback_t, std::function<void(build_context&)>);

    template <typename parent_widget_t>
    friend static void build_advanced(parent_widget_t*, std::function<void(build_advanced_context&)>, std::function<void(build_advanced_context&)>);
};

}

#include <any>

namespace staticgui {

namespace internal {
    namespace detail {

        struct runtime_context {
            // hidden behind contexts
        };

        struct runtime_widget {
            std::string typeid_name;
            bool is_stateless;
            bool is_advanced;
            std::any moved_data;
            std::function<void(runtime_context&)> create_callback;
            std::function<void(runtime_context&)> update_callback;
        };

        // tree functions

    }
}

template <typename root_widget_t>
application<root_widget_t>::application(root_widget_t&& child)
{
    internal::detail::runtime_widget _widget;
    _widget.typeid_name = typeid(root_widget_t).name();
    _widget.is_stateless = true;
    _widget.is_advanced = false;
    _widget.create_callback = [&](internal::detail::runtime_context& context) {

    };
    _widget.update_callback = nullptr;
    _widget.moved_data = child;
    internal::id::assign_in_tree(child.id, std::move(_widget));
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
static void build_stateless(parent_widget_t* parent, child_widget_t&& child)
{
    internal::detail::runtime_widget _widget;
    _widget.typeid_name = typeid(parent_widget_t).name();
    _widget.is_stateless = true;
    _widget.is_advanced = false;
    _widget.create_callback = [&](internal::detail::runtime_context& context) {

    };
    _widget.update_callback = nullptr;
    _widget.moved_data = child;
    internal::id::assign_in_tree(child.id, std::move(_widget));
    internal::id::prepare_in_tree<internal::detail::runtime_widget>(parent->id);
    internal::id::reparent_in_tree<internal::detail::runtime_widget>(parent->id, child.id);
}

template <typename parent_widget_t, typename create_widget_callback_t>
static void build_stateful(
    parent_widget_t* parent,
    create_widget_callback_t create_widget_callback,
    std::function<void(build_context&)> update_widgets_callback)
{
    std::cout << "parent id = " << parent->id << std::endl;
    build_context b;
    build_context& bb(b);
    using return_widget_t = decltype(create_widget_callback(std::forward<build_context&>(bb)));
    // build_stateless(parent, _created_widget);

    return_widget_t _created = create_widget_callback(std::forward<build_context&>(bb));
    internal::detail::runtime_widget _widget;
    _widget.typeid_name = typeid(parent_widget_t).name();
    _widget.is_stateless = false;
    _widget.is_advanced = false;
    _widget.create_callback = [&](internal::detail::runtime_context& context) {
    };
    if (!update_widgets_callback)
        _widget.update_callback = nullptr;
    else {
    }
    _widget.moved_data = std::move(_created);
    internal::id::assign_in_tree(_created.id, std::move(_widget));
    internal::id::prepare_in_tree<internal::detail::runtime_widget>(parent->id);
    internal::id::reparent_in_tree<internal::detail::runtime_widget>(parent->id, _created.id);
}

template <typename parent_widget_t>
static void build_advanced(
    parent_widget_t* parent,
    std::function<void(build_advanced_context&)>
        build_callback,
    std::function<void(build_advanced_context&)> maintain_callback)
{

    // internal::id::assign_in_tree(_created.id, std::move(_widget));
    internal::id::prepare_in_tree<internal::detail::runtime_widget>(parent->id);
    // internal::id::reparent_in_tree<internal::detail::runtime_widget>(parent->id, _created.id);
}

}

#include "widgets/center.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/future_builder.hpp"
#include "widgets/gesture_detector.hpp"
#include "widgets/image.hpp"
#include "widgets/list_view.hpp"