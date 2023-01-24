//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#define STATICGUI_DEBUG

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

    /// @brief
    /// @param stream
    /// @return
    application debug_stream(const std::ostream& stream = std::cerr);
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
void build_stateless(parent_widget_t* parent, child_widget_t&& child);

/// @brief
/// @tparam parent_widget_t
/// @tparam create_widget_callback_t
/// @param parent
/// @param create_widget_callback
/// @param update_widgets_callback
template <typename parent_widget_t, typename create_widget_callback_t>
void build_stateful(parent_widget_t* parent, create_widget_callback_t create_widget_callback, std::function<void(build_context&)> update_widgets_callback = nullptr);

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

    ~base_widget()
    {
        std::cout << "id " << id << " is going out of scope" << std::endl;
    }

private:
    template <typename root_widget_t>
    friend struct application;

    template <typename parent_widget_t>
    friend void build2(parent_widget_t* parent);

    template <typename parent_widget_t, typename child_widget_t>
    friend void build_stateless(parent_widget_t*, child_widget_t&&);

    template <typename parent_widget_t, typename create_widget_callback_t>
    friend void build_stateful(parent_widget_t*, create_widget_callback_t, std::function<void(build_context&)>);

    template <typename parent_widget_t>
    friend void build_advanced(parent_widget_t*, std::function<void(build_advanced_context&)>, std::function<void(build_advanced_context&)>);
};

#if defined(STATICGUI_DEBUG)

/// @brief
void print_build_tree();

#endif

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

        std::ostream& operator<<(std::ostream& os, const runtime_widget& o)
        {
            return os << o.typeid_name;
        }

        // tree functions

    }
}

template <typename root_widget_t>
application<root_widget_t>::application(root_widget_t&& child)
{
    // internal::detail::runtime_widget _widget;
    // _widget.typeid_name = typeid(root_widget_t).name();
    // _widget.is_stateless = true;
    // _widget.is_advanced = false;
    // _widget.create_callback = [&](internal::detail::runtime_context& context) {

    // };
    // _widget.update_callback = nullptr;
    // _widget.moved_data = std::move(child);
    // internal::id::assign_in_tree_head(child.id, std::move(_widget));

    // internal::id::reparent_at_begin<internal::detail::runtime_widget>(child.id);

    print_build_tree();

    auto it = internal::id::find_in_tree<internal::detail::runtime_widget>(3);
    internal::detail::runtime_context bb;
    std::cout << it->second.value().typeid_name << std::endl;
    auto _val = it->second.value();
    _val.update_callback(bb);
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
application<root_widget_t> application<root_widget_t>::debug_stream(const std::ostream& stream)
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
void build_stateless(parent_widget_t* parent, child_widget_t&& child)
{
    internal::detail::runtime_widget _widget;
    _widget.typeid_name = typeid(parent_widget_t).name();
    _widget.is_stateless = true;
    _widget.is_advanced = false;
    _widget.create_callback = [&](internal::detail::runtime_context& context) {

    };
    _widget.update_callback = _widget.update_callback = [&](internal::detail::runtime_context& context) {

    };
    _widget.moved_data = child;
    internal::id::assign_in_tree_head(child.id, std::move(_widget));
    if constexpr (!std::is_same_v<parent_widget_t, application<child_widget_t>>) {
        std::cout << "parent = " << typeid(parent_widget_t).name() << ", child = " << typeid(child_widget_t).name() << std::endl;
        internal::id::prepare_in_tree_head<internal::detail::runtime_widget>(parent->id);
        internal::id::reparent_in_tree_head<internal::detail::runtime_widget>(parent->id, child.id);
    } else
        std::cout << "YES\n";
}

template <typename parent_widget_t>
void build2(parent_widget_t* parent)
{
    build_context b;
    build_context& bb(b);
    using return_widget_t = decltype(parent->create(std::forward<build_context&>(bb)));

    parent->on_update(bb);
    // std::move(_created);
    internal::detail::runtime_widget _widget;
    _widget.typeid_name = typeid(parent_widget_t).name();
    _widget.is_stateless = false;
    _widget.is_advanced = false;
    _widget.moved_data.emplace<return_widget_t>(parent->create((bb)));
    auto& _child_data = std::any_cast<return_widget_t&>(_widget.moved_data);
    parent->on_update(bb);

    _widget.update_callback = [&](internal::detail::runtime_context& context) {
        parent->on_update(bb);
        // _widget.moved_data
        // build_context b;
        std::cout << "update stateful\n";
        // update_widgets_callback(b);
    };

    internal::id::assign_in_tree_head(_child_data.id, std::move(_widget));
    internal::id::prepare_in_tree_head<internal::detail::runtime_widget>(parent->id);
    internal::id::reparent_in_tree_head<internal::detail::runtime_widget>(parent->id, _child_data.id);

    // internal::detail::runtime_context ctx;

    // _widget.create_callback(ctx);
    // if (!update_widgets_callback)
    //     _widget.update_callback = nullptr;
    // else {

    // }
}

template <typename parent_widget_t, typename create_widget_callback_t>
void build_stateful(
    parent_widget_t* parent,
    create_widget_callback_t create_widget_callback,
    std::function<void(build_context&)> update_widgets_callback)
{
    std::cout << "parent id = " << parent->id << std::endl;
    build_context b;
    build_context& bb(b);
    using return_widget_t = decltype(create_widget_callback(std::forward<build_context&>(bb)));
    // build_stateless(parent, _created_widget);

    internal::detail::runtime_widget _widget;
    _widget.moved_data = create_widget_callback(std::forward<build_context&>(bb));
    return_widget_t& _data = std::any_cast<return_widget_t&>(_widget.moved_data);
    _widget.typeid_name = typeid(parent_widget_t).name();
    _widget.is_stateless = false;
    _widget.is_advanced = false;
    // std::move(_created);
    _widget.create_callback = [&](internal::detail::runtime_context& context) {
    };
    if (!update_widgets_callback)
        _widget.update_callback = nullptr;
    else {
        _widget.update_callback = [&](internal::detail::runtime_context& context) {
            // _widget.moved_data
            // build_context b;
            std::cout << "update stateful\n";
            update_widgets_callback(b);
        };
    }

    internal::id::assign_in_tree_head(_data.id, std::move(_widget));
    if constexpr (!std::is_same_v<parent_widget_t, application<return_widget_t>>) {
        std::cout << "parent = " << typeid(parent_widget_t).name() << ", child = " << typeid(return_widget_t).name() << std::endl;
        internal::id::prepare_in_tree_head<internal::detail::runtime_widget>(parent->id);
        internal::id::reparent_in_tree_head<internal::detail::runtime_widget>(parent->id, _data.id);
    } else
        std::cout << "YES\n";
}

template <typename parent_widget_t>
void build_advanced(
    parent_widget_t* parent,
    std::function<void(build_advanced_context&)>
        build_callback,
    std::function<void(build_advanced_context&)> maintain_callback)
{

    // internal::id::assign_in_tree(_created.id, std::move(_widget));
    // internal::id::prepare_in_tree_head<internal::detail::runtime_widget>(parent->id, child.id);
    // internal::id::reparent_in_tree<internal::detail::runtime_widget>(parent->id, _created.id);
}

#if defined(STATICGUI_DEBUG)

void print_build_tree()
{
    internal::id::print_tree<internal::detail::runtime_widget>();
}

#endif
}

#include "widgets/center.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/future_builder.hpp"
#include "widgets/gesture_detector.hpp"
#include "widgets/image.hpp"
#include "widgets/list_view.hpp"