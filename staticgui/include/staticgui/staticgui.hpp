//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <array>
#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

// #include <staticgui/utils/traits.hpp>

/// @brief
/// @details
#define STATICGUI_WIDGET(widget_t)

/// @brief
/// @details
namespace staticgui {

/// @brief
namespace tools {

    /// @brief
    /// @tparam function_t
    /// @tparam start_t
    /// @tparam end_t
    /// @tparam increment_t
    /// @param function
    template <unsigned int start_t, unsigned int end_t, unsigned int increment_t, typename function_t>
    constexpr void constexpr_for(function_t&& function);

    /// @brief
    /// @tparam ...values_t
    /// @tparam function_t
    /// @param function
    /// @param ...values
    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, values_t&... values);

    /// @brief
    /// @tparam ...values_t
    /// @tparam function_t
    /// @param function
    /// @param ...values
    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, const values_t&... values);
}

/// @brief
namespace traits {

    // template <typename... values_t>
    // struct first_value;

    template <typename first_value_t, typename... other_values_t>
    struct first_value_impl {
        using type = first_value_t;
    };

    // /// @brief
    // /// @tparam value_t
    // template <typename value_t>
    // constexpr bool has_add();

    // /// @brief
    // /// @tparam value_t
    // template <typename value_t>
    // constexpr bool has_float_multiply();

    template <typename value_t>
    constexpr bool has_add_v = (internal::traits::has_outside_add<value_t> || internal::traits::has_inside_add<value_t>);

    template <typename value_t>
    constexpr bool has_float_multiply_v = (internal::traits::has_outside_float_left_multiply<value_t> || internal::traits::has_outside_float_right_multiply<value_t> || internal::traits::has_inside_float_multiply<value_t>);

    /// @brief
    /// @tparam value_t
    template <typename value_t>
    constexpr bool is_lerpable = (has_float_multiply_v<value_t> && has_add_v<value_t>);

    /// @brief
    /// @tparam value_t
    template <typename value_t>
    using lerpable_value_t = typename std::enable_if_t<is_lerpable<value_t>, value_t>;

    /// @brief
    /// @tparam function_t
    /// @tparam ...args_t
    template <typename function_t, typename... values_t>
    constexpr bool is_convertible_to_callback();

    /// @brief
    /// @tparam value_t
    template <typename function_t, typename... values_t>
    using callback_function_t = typename std::enable_if_t<is_convertible_to_callback<function_t, values_t...>, function_t>;
}

namespace internal {
    namespace impl {
        struct event_impl;
        struct curve_impl;
        struct animation_impl;
        struct value_impl;
        struct context_impl;
        struct layout_impl;
    }
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

    template <typename function_t>
    event(function_t&& function);

    event(const event& other) = delete;

    event& operator=(const event& other) = delete;

    event(event&& other);

    event& operator=(event&& other);

    /// @brief
    /// @param trigger_callback
    /// @return
    event& on_trigger(std::function<void(const values_t&...)> trigger_callback);

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
    internal::impl::event_impl& _impl;
};

/// @brief
/// @tparam value_t
template <typename value_t>
struct animation {

    /// @brief
    /// @param ...curves
    animation(const curve<value_t>& bezier_curve);

    animation(const animation& other) = delete;

    animation& operator=(const animation& other) = delete;

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

    /// @brief
    /// @param animated_value
    value(const animation<traits::lerpable_value_t<value_t>>& animated_value);

    /// @brief
    /// @param static_value
    value(const traits::lerpable_value_t<value_t>& static_value);

    /// @brief
    /// @param target_value
    void assign(value_t& target_value) const; // sets or registers animation callback ^^

private:
    internal::impl::value_impl& _impl;
};

/// @brief
struct context {

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
private:
    internal::impl::context_impl& _impl;
};

struct layout {
    //     // cursor etc

    //     // im gui api here

    template <typename widget_t>
    layout& build_here(widget_t& widget);

private:
    internal::impl::layout_impl& _impl;
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
/// @return
context& get_context();

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
void build_advanced(widget_t* widget, children_widgets_t&... children_widgets, std::function<void(layout&)> context_callback, const bool is_above_root_widgets = false);

// template <typename widget_t, typename... children_widgets_t>
// void build_advanced(widget_t& widget, children_widgets_t&... children_widgets, const event<layout&> context_callback);

// #if defined(STATICGUI_DEBUG)

/// @brief
void print_build_tree();

// #endif
}

#include "staticgui.inl"

#include <staticgui/impl/animation.inl>
#include <staticgui/impl/build.inl>
#include <staticgui/impl/context.inl>
#include <staticgui/impl/curve.inl>
#include <staticgui/impl/event.inl>
#include <staticgui/impl/layout.inl>
#include <staticgui/impl/lerp.inl>
#include <staticgui/impl/make.inl>
#include <staticgui/impl/tools.inl>
#include <staticgui/impl/traits.inl>
#include <staticgui/impl/value.inl>
#include <staticgui/impl/widgets.inl>

