//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <future>
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
/// @param what
void throw_error(const std::string& what);

/// @brief
/// @details
enum struct curve_preset {
    linear,
    bounce_in,
};

/// @brief Curve object
/// @details Curves are represented by bsplines and can be created directly from control points.
struct curve {
    curve(const curve_preset preset);
    curve(const float departure, const float arrival, const std::vector<float2>& controls = {});

private:
    detail::curve_data _data;
    template <typename value_t>
    struct animation;
};

/// @brief Event objects store same type callbacks and can be passed around the user code to be
/// triggered all at once when desired. Events can be fired on std::future completion, taking its
/// return value as input parameters for callbacks
/// @details Lifetime
/// @tparam ...values_t
template <typename... values_t>
struct event {

    /// @brief
    using on_trigger_callback = std::function<void(const values_t&...)>;

    /// @brief
    using future_values = glue::future_typelist_t<values_t...>;

    event();
    event(const on_trigger_callback& trigger_callback);
    event(const event& other);
    event& operator=(const event& other);
    event(event&& other);
    event& operator=(event&& other);
    ~event();

    /// @brief
    /// @details
    event& merge(const event& other);

    /// @brief
    /// @details
    event& operator+=(const event& other);

    /// @brief Transfers ownership of the underlying callback dispatcher back to this event object
    /// @details This allows you to
    event& attach();

    /// @brief Transfers ownership of the underlying callback dispatcher to the specified widget
    /// @details This allows you to let this event object exit scope without destroying its
    /// callback dispatch subroutine. While an event is detached to a widget both will be destroyed
    /// at the same time
    /// @tparam widget_t
    /// @param widget
    template <typename widget_t>
    event& detach(widget_t& widget);

    /// @brief Transfers ownership of the underlying callback dispatcher to the internal registry
    /// @details This allows you to let this event object exit scope without destroying its
    /// callback dispatch subroutine. While an event is detached to the internal registry it will
    /// be destroyed when the application terminates
    event& detach();

    /// @brief Emplaces a new callback to be fired after
    /// @param trigger_callback
    event& on_trigger(const on_trigger_callback& trigger_callback);

    /// @brief Triggers all contained callbacks
    /// @param ...values
    const event& trigger(values_t&&... values) const; // + operator() overload

    /// @brief
    /// @param future_value
    void trigger(std::future<future_values>&& future_value); // + operator() overload

    /// @brief
    /// @param future_value
    void trigger(const std::shared_future<future_values>& shared_future_value); // + operator() overload

    /// @brief
    std::vector<on_trigger_callback>& trigger_callbacks();

    /// @brief
    const std::vector<on_trigger_callback>& trigger_callbacks() const;

private:
    detail::event_impl<values_t...>& _impl;
    template <typename value_t>
    struct animation;
};

/// @brief
/// @details
enum struct animation_mode {
    forward,
    backward,
    repeat,
    loop,
    random
};

/// @brief
/// @tparam value_t
template <typename value_t>
struct animation {

    animation();
    template <typename duration_unit_t = std::chrono::seconds>
    animation(const curve& curved_shape, const unsigned int duration_count = 1, const animation_mode mode = animation_mode::forward);
    animation(const animation& other);
    animation& operator=(const animation& other);
    animation(animation&& other);
    animation& operator=(animation&& other);
    ~animation();

    /// @brief
    /// @param value_changed_event
    animation& on_value_changed(const event<value_t>& value_changed_event);

    /// @brief
    animation& start();

    /// @brief
    animation& stop();

    /// @brief
    animation& reset();

    /// @brief
    animation& shape(const curve& curved_shape);

    /// @brief
    template <typename duration_unit_t = std::chrono::seconds>
    animation& duration(const unsigned int count);

    /// @brief Transfers ownership of the underlying callback dispatcher back to this animation object
    /// @details This allows you to
    animation& attach();

    /// @brief Transfers ownership of the underlying callback dispatcher to the specified widget
    /// @details This allows you to let this animation object exit scope without destroying its
    /// callback dispatch subroutine. While an animation is detached to a widget both will be destroyed
    /// at the same time
    /// @tparam widget_t
    /// @param widget
    template <typename widget_t>
    animation& detach(widget_t& widget);

    /// @brief Transfers ownership of the underlying callback dispatcher to the internal registry
    /// @details This allows you to let this animation object exit scope without destroying its
    /// callback dispatch subroutine. While an animation is detached to the internal registry it will
    /// be destroyed when the application terminates
    animation& detach();

private:
    detail::animation_impl<value_t>& _impl;
};

/// @brief
/// @tparam value_t
template <typename value_t>
struct animatable {

    animatable(const animation<detail::enable_if_lerpable_t<value_t>>& animated_value);
    animatable(const detail::enable_if_lerpable_t<value_t>& value);
    animatable(const animatable& other);
    animatable& operator=(const animatable& other);
    animatable(animatable&& other);
    animatable& operator=(animatable&& other);
    ~animatable();

    /// @brief
    /// @param target_value
    void assign(value_t& target_value) const; // sets or registers animation callback + detach ^^

private:
    // bool _is_animated;
    // mutable std::variant<value_t, animation<value_t>> _value;
};

/// @brief
/// @details
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
std::function<void()> launch_embedded(widget_t& widget);

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

/// @brief
/// @details
struct resolve_constraint {
    // getters
};

/// @brief
/// @details
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
/// @details
struct draw_rounding_command {

    draw_rounding_command& strength(const float z);

    draw_rounding_command& top_left(const bool enable);

    draw_rounding_command& top_right(const bool enable);

    draw_rounding_command& bottom_left(const bool enable);

    draw_rounding_command& bottom_right(const bool enable);

private:
    detail::rounding_data _rounding;
    friend struct draw_rectangle_command;
};

/// @brief
/// @details
struct draw_line_command {
    draw_line_command& first_point(const float2& first);

    draw_line_command& second_point(const float2& second);

    draw_line_command& color(const float4& col);

    draw_line_command& thickness(const float t = 10.f);

private:
    detail::line_command_data _line_command_data;
    friend struct draw_command;
};

/// @brief
/// @details
struct draw_rectangle_command {
    draw_rectangle_command& min_point(const float2& first);

    draw_rectangle_command& max_point(const float2& second);

    draw_rectangle_command& color(const float4& col);

    draw_rectangle_command& rounding(const draw_rounding_command& rounding_command);

    draw_rectangle_command& thickness(const float t = 10.f);

private:
    detail::rectangle_command_data _rectangle_command_data;
    friend struct draw_command;
};

/// @brief
/// @details
struct draw_command {
    draw_command(detail::command_data& data);
    draw_command(const draw_command& other);
    draw_command& operator=(const draw_command& other);
    draw_command(draw_command&& other);
    draw_command& operator=(draw_command&& other);

    draw_command& add_line(const draw_line_command& line_command);

    draw_command& add_rectangle(const draw_rectangle_command& rectangle_command);

private:
    detail::command_data& _command_data;
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