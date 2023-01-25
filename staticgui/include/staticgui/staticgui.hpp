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
struct application {

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

    /// @brief
    /// @param stream
    /// @return
    application& debug_stream(const std::ostream& stream = std::cerr);
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
application& launch(widget_t& widget);

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
application& attach(widget_t& widget);

/// @brief
/// @tparam widget_t
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return
template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args);

/// @brief
/// @tparam widget_t
/// @tparam child_widget_t
/// @param widget
/// @param child_widget
template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget);

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
struct build_advanced_context : public build_context {

    // cursor etc

    template <typename root_widget_t>
    static void append(const root_widget_t& root) { }
};

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
    friend parent_widget_t& build2(parent_widget_t* parent);

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
            std::any moved_data = nullptr; // instance
            std::function<void(runtime_context&)> create_callback = nullptr;
            std::function<void(runtime_context&)> update_callback = nullptr;
        };

    }
}

// template <typename root_widget_t>
// application<root_widget_t>::application(root_widget_t&& child)
// {
//     // internal::detail::runtime_widget _widget;
//     // _widget.typeid_name = typeid(root_widget_t).name();
//     // _widget.is_stateless = true;
//     // _widget.is_advanced = false;
//     // _widget.create_callback = [&](internal::detail::runtime_context& context) {

//     // };
//     // _widget.update_callback = nullptr;
//     // _widget.moved_data = std::move(child);
//     // internal::id::assign_in_tree_head(child.id, std::move(_widget));

//     // internal::id::reparent_at_begin<internal::detail::runtime_widget>(child.id);

//     print_build_tree();

//     auto k = std::async(std::launch::async, [&]() {
//         std::this_thread::sleep_for(std::chrono::seconds { 1 });

//         auto it = internal::id::find_in_tree<internal::detail::runtime_widget>(2);
//         // build_context b;
//         internal::detail::runtime_context bb;
//         // child.update(b);
//         std::cout << it->second.typeid_name << std::endl;
//         auto _val = it->second;
//         std::any_cast<mywidget&>(_val.moved_data).set_x(666.f);
//         _val.update_callback(bb);
//     });

//     auto k2 = std::async(std::launch::async, [&]() {
//         std::this_thread::sleep_for(std::chrono::seconds { 2 });

//         auto it = internal::id::find_in_tree<internal::detail::runtime_widget>(2);
//         // build_context b;
//         internal::detail::runtime_context bb;
//         // child.update(b);
//         std::cout << it->second.typeid_name << std::endl;
//         auto _val = it->second;
//         _val.update_callback(bb);
//     });
//     auto k3 = std::async(std::launch::async, [&]() {
//         std::this_thread::sleep_for(std::chrono::seconds { 3 });

//         auto it = internal::id::find_in_tree<internal::detail::runtime_widget>(2);
//         // build_context b;
//         internal::detail::runtime_context bb;
//         // child.update(b);
//         std::cout << it->second.typeid_name << std::endl;
//         auto _val = it->second;
//         _val.update_callback(bb);
//     });
// }

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

template <typename widget_t>
application& launch(widget_t& widget)
{
}

template <typename widget_t>
application& attach(widget_t& widget)
{
}

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    // allocates a new widget inside the runtime_widget
    internal::detail::runtime_widget& _widget = internal::id::emplace_in_tree<internal::detail::runtime_widget>(0);

    return std::any_cast<widget_t&>(_widget.moved_data);
}

template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget)
{
}

template <typename parent_widget_t>
parent_widget_t& build2(parent_widget_t* parent)
{
    build_context _context;
    using child_widget_t = decltype(parent->create(_context));
    child_widget_t& _child = parent->create(_context); // trigger build on descendants

    std::cout << "build2 - " << typeid(parent_widget_t).name() << std::endl;

    internal::detail::runtime_widget& _widget = internal::id::emplace_in_tree<internal::detail::runtime_widget>(parent->id);

    // internal::detail::runtime_widget& _widget_ref;
    _widget.typeid_name = typeid(parent_widget_t).name();
    _widget.moved_data.emplace<parent_widget_t&>(std::ref(*parent));
    std::cout << (parent)->_x << std::endl;
    parent = &(std::any_cast<parent_widget_t&>(_widget.moved_data));
    _widget.update_callback = [&](internal::detail::runtime_context& context) {
        std::any_cast<parent_widget_t&>(_widget.moved_data).update(_context);
        std::cout << "update stateful\n";
        std::any_cast<parent_widget_t&>(_widget.moved_data)._x += 5.f;
    };

    // internal::id::reparent_in_tree_head<internal::detail::runtime_widget>(parent->id, _child.id);
    parent = &(std::any_cast<parent_widget_t&>(_widget.moved_data));
    // internal::id::prepare_in_tree_head<internal::detail::runtime_widget>(parent->id);
    // internal::id::assign_in_tree_head(parent->id, _widget);
    return (std::any_cast<parent_widget_t&>(_widget.moved_data));
}

#if defined(STATICGUI_DEBUG)

void print_build_tree()
{
    internal::id::print_tree<internal::detail::runtime_widget>();
    // internal::detail::runtime_context _rc;
    // _it->second.update_callback(_rc);
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