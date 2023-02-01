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
#include <array>
#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeindex>
#include <variant>
#include <vector>

#include <staticgui/glue/any_function.hpp>
#include <staticgui/glue/constexpr_for.hpp>
#include <staticgui/glue/graphics.hpp>
#include <staticgui/glue/group.hpp>
#include <staticgui/glue/id_integer.hpp>
#include <staticgui/glue/is_lerpable.hpp>
#include <staticgui/glue/platform.hpp>
#include <staticgui/glue/registry.hpp>
#include <staticgui/glue/tree.hpp>
#include <staticgui/glue/typelist.hpp>

#include <staticgui/containers/widgets_registry.hpp>

namespace internal {
namespace impl {
    // struct any_function_impl;
    struct event_component;
    struct curve_impl;
    struct animation_impl;
    struct value_impl;
    struct context_impl;
    struct context_manager;
    struct layout_impl;
    struct layout_manager;

}
}

/// @brief
/// @details
// #define STATICGUI_WIDGET(widget_t)

/// @brief
/// @details
namespace staticgui {

namespace detail {

    inline static widgets_registry global_widgets;

}

/// @brief
namespace traits {

    // template <typename... values_t>
    // struct first_value;

    template <typename first_value_t, typename... other_values_t>
    struct first_value_impl {
        using type = first_value_t;
    };

    /// @brief
    /// @tparam value_t
    template <typename value_t>
    using lerpable_value_t = typename std::enable_if_t<glue::is_lerpable_v<value_t>, value_t>;
}

/// @brief
/// @details
/// @tparam value_t
template <typename value_t>
traits::lerpable_value_t<value_t> lerp(const value_t& min, const value_t& max, const float t) { return value_t {}; }

/// @brief
struct color {
    color operator+(const color& other);
    color operator*(const float multiplier);
};

template <typename value_t>
struct curve {
    curve(const traits::lerpable_value_t<value_t>& min, const value_t& max);

    std::vector<std::pair<float, value_t>>& get_points();

    const std::vector<std::pair<float, value_t>>& get_points() const;

    value_t get_value(const float t);

private:
    internal::impl::curve_impl& _impl;
};

/// @brief
/// @tparam ...values_t
template <typename... values_t>
struct event {
    event();
    event(const std::function<void(values_t...)>& trigger_callback);
    event(const event& other);
    event& operator=(const event& other);
    event(event&& other);
    event& operator=(event&& other);

    /// @brief
    /// @param trigger_callback
    /// @return
    event& on_trigger(const std::function<void(const values_t&...)>& trigger_callback);

    event& clear();

    /// @brief
    /// @param ...values
    void trigger(const values_t&... values) const;

    /// @brief
    /// @param future_value
    template <typename = typename std::enable_if_t<sizeof...(values_t) == 0>>
    void trigger(const std::future<void>& future_value);

    /// @brief
    /// @param future_value
    template <typename = typename std::enable_if_t<sizeof...(values_t) == 1>>
    void trigger(const std::future<typename traits::first_value_impl<values_t...>::type>& future_value);

    /// @brief
    /// @param future_value
    template <typename = typename std::enable_if_t<sizeof...(values_t) >= 2>>
    void trigger(const std::future<std::tuple<values_t...>>& future_value);

private:
    // detail::event_component& _impl;
};

// template <typename... values_t>
// event(const std::function<void(values_t...)>&) -> event<values_t...>;

/// @brief
/// @tparam value_t
template <typename value_t>
struct animation {
    animation(const curve<value_t>& bezier_curve);
    animation(const animation& other);
    animation& operator=(const animation& other);
    animation(animation&& other);
    animation& operator=(animation&& other);

    /// @brief
    /// @param value_changed_event
    /// @return
    animation& on_value_changed(const event<value_t>& value_changed_event);

    /// @brief
    /// @tparam other_value_t
    /// @param previous
    /// @return
    template <typename other_value_t>
    animation& start_after(const animation<other_value_t>& previous);

    /// @brief
    void start();

    void stop();

private:
    internal::impl::animation_impl& _impl;
};

/// @brief
/// @tparam value_t
template <typename value_t>
struct value {
    value(const animation<traits::lerpable_value_t<value_t>>& animated_value);
    value(const traits::lerpable_value_t<value_t>& static_value);

    /// @brief
    /// @param target_value
    void assign(value_t& target_value) const; // sets or registers animation callback ^^

private:
    internal::impl::value_impl& _impl;
};

/// @brief
struct context {

    template <typename widget_t>
    context& destroy(widget_t& widget, const bool destroy_children = true);

    /// @brief
    /// @param title
    /// @return
    context& window_title(const std::string& title);

    /// @brief
    /// @param width
    /// @param height
    /// @return
    context& window_size(const unsigned int width, const unsigned int height);

    /// @brief
    /// @param window_resized_event
    /// @return
    context& on_window_resized(const event<float, float>& window_resized_event);

    /// @brief
    /// @param stream
    /// @return
    context& debug_stream(const std::ostream& stream = std::cerr);

    /// @brief
    /// @tparam widget_t
    /// @param named_route_widget
    template <typename widget_t>
    context& create_route(std::pair<std::string, widget_t&> named_route_widget);

    /// @brief
    /// @param named_route
    context& destroy_route(const std::string& named_route);

    /// @brief
    /// @param navigate_event
    context& on_navigate_to(const event<float>& navigate_event);

    /// @brief
    /// @param named_route
    /// @param navigate_event
    context& on_navigate_to(const std::string& named_route, const animation<float>& navigate_event);

    /// @brief
    /// @param navigate_event
    context& on_navigate_from(const event<float>& navigate_event);

    /// @brief
    /// @param named_route
    /// @param navigate_event
    context& on_navigate_from(const std::string& named_route, const animation<float>& navigate_event);

    /// @brief
    /// @param named_route
    /// @param transition_animation
    context& navigate_forwards(const std::string& named_route);

    /// @brief
    /// @param named_route
    /// @param transition_animation
    context& navigate_backwards();

    // input avec callbacks pareil

    context& show_virtual_keyboard(const bool show);

    context& on_key_pressed(const event<void>& key_pressed_callback);

    // navigation ?

    inline void erase() { }

    inline void erase_children() { }

    template <typename widget_t>
    void replace(const widget_t& widget) { }

    template <typename child_widget_t, typename widget_t>
    void replace(const child_widget_t& child_widget, const widget_t& widget) { }

    template <typename widget_t>
    void replace_children(const widget_t& widget) { }

    inline void maintain() { }

    template <typename child_widget_t>
    void maintain_child(const child_widget_t& child) { }

    inline void maintain_children() { }

    // rebuild
    // private:
    context();
    friend context& get_context();
};

/// @brief
/// @return
context& get_context();

struct layout {
    //     // cursor etc

    //     // im gui api here

    template <typename widget_t>
    layout& build_here(widget_t& widget);

private:
    layout();
    friend struct internal::impl::layout_manager;
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
void launch(widget_t& widget);

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
void attach(widget_t& widget);

/// @brief
/// @tparam widget_t
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args);

/// @brief
/// @tparam widget_t
/// @tparam child_widget_t
/// @param widget
/// @param child_widget
template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget, const bool is_above_navigation = false);

// template <typename widget_t, typename child_widget_t>
// void build(widget_t& widget, child_widget_t& child_widget);

/// @brief
/// @tparam parent_widget_t
/// @param  widget
/// @param  paint_callback
template <typename widget_t, typename... children_widgets_t>
void build_advanced(widget_t* widget, std::function<void(layout&)> context_callback, children_widgets_t&... children_widgets);

// #if defined(STATICGUI_DEBUG)

/// @brief
void print_build_tree();

// #endif

}

#include <staticgui/impl/animation.inl>
#include <staticgui/impl/attach.inl>
#include <staticgui/impl/build.inl>
#include <staticgui/impl/context.inl>
#include <staticgui/impl/curve.inl>
#include <staticgui/impl/event.inl>
#include <staticgui/impl/launch.inl>
#include <staticgui/impl/layout.inl>
#include <staticgui/impl/lerp.inl>
#include <staticgui/impl/make.inl>
#include <staticgui/impl/value.inl>
#include <staticgui/impl/widgets.inl>

#include <staticgui/widgets/center.hpp>
#include <staticgui/widgets/column.hpp>
#include <staticgui/widgets/container.hpp>
#include <staticgui/widgets/future_builder.hpp>
#include <staticgui/widgets/gesture_detector.hpp>
#include <staticgui/widgets/image.hpp>
#include <staticgui/widgets/list_view.hpp>