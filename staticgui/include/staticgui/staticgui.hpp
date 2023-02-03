//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <iostream>

#include <staticgui/state/host.hpp>
#include <staticgui/state/lerpable.hpp>

using int2 = staticgui::glue::simd_array<int, 2>;
using int3 = staticgui::glue::simd_array<int, 3>;
using int4 = staticgui::glue::simd_array<int, 4>;
using int3x3 = staticgui::glue::simd_ordered_array<int, 9, staticgui::glue::simd_order::row_major>;
using int4x4 = staticgui::glue::simd_ordered_array<int, 16, staticgui::glue::simd_order::row_major>;
using float2 = staticgui::glue::simd_array<float, 2>;
using float3 = staticgui::glue::simd_array<float, 3>;
using float4 = staticgui::glue::simd_array<float, 4>;
using float3x3 = staticgui::glue::simd_ordered_array<float, 9, staticgui::glue::simd_order::row_major>;
using float4x4 = staticgui::glue::simd_ordered_array<float, 16, staticgui::glue::simd_order::row_major>;
using double2 = staticgui::glue::simd_array<double, 2>;
using double3 = staticgui::glue::simd_array<double, 3>;
using double4 = staticgui::glue::simd_array<double, 4>;
using double3x3 = staticgui::glue::simd_ordered_array<double, 9, staticgui::glue::simd_order::row_major>;
using double4x4 = staticgui::glue::simd_ordered_array<double, 16, staticgui::glue::simd_order::row_major>;

/// @brief
/// @details
namespace staticgui {

/// @brief
/// @details
/// @tparam value_t
template <typename value_t>
detail::enable_if_lerpable_t<value_t> lerp(const value_t& min, const value_t& max, const float t) { return value_t {}; }

/// @brief
struct color {
    color operator+(const color& other);
    color operator*(const float multiplier);
};

template <typename value_t>
struct curve {
    curve(const detail::enable_if_lerpable_t<value_t>& min, const value_t& max);

    std::vector<std::pair<float, value_t>>& get_points();

    const std::vector<std::pair<float, value_t>>& get_points() const;

    value_t get_value(const float t);

private:
    // internal::impl::curve_impl& _impl;
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

    // /// @brief
    // /// @param future_value
    // template <typename = typename std::enable_if_t<sizeof...(values_t) == 0>>
    // void trigger(const std::future<void>& future_value);

    // /// @brief
    // /// @param future_value
    // template <typename = typename std::enable_if_t<sizeof...(values_t) == 1>>
    // void trigger(const std::future<typename traits::first_value_impl<values_t...>::type>& future_value);

    // /// @brief
    // /// @param future_value
    // template <typename = typename std::enable_if_t<sizeof...(values_t) >= 2>>
    // void trigger(const std::future<std::tuple<values_t...>>& future_value);

private:
    detail::event_impl<values_t...>& _impl;
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

    // /// @brief
    // /// @tparam other_value_t
    // /// @param previous
    // /// @return
    // template <typename other_value_t>
    // animation& start_after(const animation<other_value_t>& previous);

    /// @brief
    void start();

    void stop();

private:
    detail::animation_impl<value_t>& _impl;
};

/// @brief
/// @tparam value_t
template <typename value_t>
struct value {
    value(const animation<detail::enable_if_lerpable_t<value_t>>& animated_value);
    value(const detail::enable_if_lerpable_t<value_t>& static_value);

    /// @brief
    /// @param target_value
    void assign(value_t& target_value) const; // sets or registers animation callback ^^

private:
    bool _is_animated;
    mutable std::variant<value_t, animation<value_t>> _value;
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

private:
    // layout();
    // friend struct internal::impl::layout_manager;
};

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void launch(widget_t& widget);

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
std::function<void()> attach(widget_t& widget);

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
template <typename widget_t, typename... children_widgets_t>
void declare(widget_t* widget, children_widgets_t&... children_widgets);

struct resolve_constraint {
    // getters
};

struct resolve_advice {

    // setters
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param resolve_callback
template <typename widget_t>
void on_resolve(widget_t* widget, const std::function<void(const resolve_constraint&, resolve_advice&)>& resolve_callback);

/// @brief
struct draw_rounding_command {

    draw_rounding_command& strength(const float z);

    draw_rounding_command& top_left(const bool enable);

    draw_rounding_command& top_right(const bool enable);

    draw_rounding_command& bottom_left(const bool enable);

    draw_rounding_command& bottom_right(const bool enable);

private:
    detail::rounding_data _rounding;
};

struct draw_line_command {
    glue::simd_array<float, 2> first_point;
    glue::simd_array<float, 2> second_point;
    glue::simd_array<float, 4> color;
    float thickness = 1.f;
};

struct draw_rectangle_command {
    glue::simd_array<float, 2> min_point;
    glue::simd_array<float, 2> max_point;
    glue::simd_array<float, 4> color;
    draw_rounding_command rounding;
    float thickness = 1.f;
};

/// @brief
struct draw_command {

    draw_command& add_line(const draw_line_command& line_command);

    draw_command& add_rectangle(const draw_rectangle_command& line_command);
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param draw_callback
template <typename widget_t>
void on_draw(widget_t* widget, const std::function<void(draw_command&)>& draw_callback);

}

#include <staticgui/staticgui.inl>

#include <staticgui/widgets/center.hpp>
#include <staticgui/widgets/column.hpp>
#include <staticgui/widgets/container.hpp>
#include <staticgui/widgets/future_builder.hpp>
#include <staticgui/widgets/gesture_detector.hpp>
#include <staticgui/widgets/image.hpp>
#include <staticgui/widgets/list_view.hpp>