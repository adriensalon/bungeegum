//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/infinity.hpp>
#include <staticgui/state/host.hpp>

template <typename value_t>
constexpr value_t infinity = staticgui::detail::infinity<value_t>;

using int2 = staticgui::detail::simd_array<int, 2>;
using int3 = staticgui::detail::simd_array<int, 3>;
using int4 = staticgui::detail::simd_array<int, 4>;
using int3x3 = staticgui::detail::simd_ordered_array<int, 9, staticgui::detail::simd_order::row_major>;
using int4x4 = staticgui::detail::simd_ordered_array<int, 16, staticgui::detail::simd_order::row_major>;
using float2 = staticgui::detail::simd_array<float, 2>;
using float3 = staticgui::detail::simd_array<float, 3>;
using float4 = staticgui::detail::simd_array<float, 4>;
using float3x3 = staticgui::detail::simd_ordered_array<float, 9, staticgui::detail::simd_order::row_major>;
using float4x4 = staticgui::detail::simd_ordered_array<float, 16, staticgui::detail::simd_order::row_major>;
using double2 = staticgui::detail::simd_array<double, 2>;
using double3 = staticgui::detail::simd_array<double, 3>;
using double4 = staticgui::detail::simd_array<double, 4>;
using double3x3 = staticgui::detail::simd_ordered_array<double, 9, staticgui::detail::simd_order::row_major>;
using double4x4 = staticgui::detail::simd_ordered_array<double, 16, staticgui::detail::simd_order::row_major>;

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
    curve(const curve& other);
    curve& operator=(const curve& other);
    curve(curve&& other);
    curve& operator=(curve&& other);

private:
    detail::curve_data _data;
    template <typename value_t>
    friend struct animation;
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
    using future_values = detail::future_typelist_t<values_t...>;

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
};

/// @brief
template <typename value_t>
value_t lerp(detail::enable_if_lerpable_t<value_t>&& min_value, value_t&& max_value, const float t);

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

    using lerpable_value = detail::enable_if_lerpable_t<value_t>;

    /// @brief
    using on_tick_callback = std::function<void(const value_t&)>;

    template <typename duration_unit_t = std::chrono::seconds>
    animation(
        const curve& curved_shape,
        lerpable_value&& min_value,
        lerpable_value&& max_value,
        const unsigned int duration_count = 1,
        const animation_mode mode = animation_mode::forward);
    animation(const animation& other);
    animation& operator=(const animation& other);
    animation(animation&& other);
    animation& operator=(animation&& other);
    ~animation();

    /// @brief
    /// @param value_changed_event
    animation& on_value_changed(const event<value_t>& value_changed_event);

    /// @brief
    /// @param value_changed_event
    animation& on_tick(const on_tick_callback& tick_callback);

    /// @brief
    /// @param value
    animation& on_tick(value_t& value, const bool must_draw = false, const bool must_resolve = false);

    /// @brief
    animation& start();

    /// @brief
    animation& stop();

    /// @brief
    animation& reset();

    /// @brief
    animation& shape(const curve& curved_shape);

    /// @brief
    animation& min(value_t&& min_value);

    /// @brief
    animation& max(value_t&& min_value);

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
    template <typename value_t>
    friend struct animatable;
};

/// @brief
/// @tparam value_t
template <typename value_t>
struct animatable {

    using lerpable_value = detail::enable_if_lerpable_t<value_t>;

    animatable(const animation<value_t>& animated_value);
    animatable(const value_t& value);
    animatable(const animatable& other);
    animatable& operator=(const animatable& other);
    animatable(animatable&& other);
    animatable& operator=(animatable&& other);

    /// @brief
    /// @param target_value
    template <typename widget_t>
    void assign(widget_t* widget, value_t& target_value) const; // sets or registers animation callback + detach ^^

private:
    detail::animatable_data<value_t> _data;
};

/// @brief
/// @details
struct context {

    /// @brief
    context& max_fps(const unsigned int fps);

    template <typename widget_t>
    context& must_resolve(widget_t* widget);

    template <typename widget_t>
    context& must_draw(widget_t* widget);

    //
    //
    //
    //
    //
    //
    //
    //

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

    // // navigation ?

    // inline void erase() { }

    // inline void erase_children() { }

    // template <typename widget_t>
    // void replace(const widget_t& widget) { }

    // template <typename child_widget_t, typename widget_t>
    // void replace(const child_widget_t& child_widget, const widget_t& widget) { }

    // template <typename widget_t>
    // void replace_children(const widget_t& widget) { }

    // inline void maintain() { }

    // template <typename child_widget_t>
    // void maintain_child(const child_widget_t& child) { }

    // inline void maintain_children() { }

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
void launch(widget_t& widget, const std::function<void()>& on_renderer_started = nullptr);

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
/// @details https://api.flutter.dev/flutter/rendering/BoxConstraints-class.html
struct resolve_constraint {
    resolve_constraint(const float2& min_size = { 0.f, 0.f }, const float2& max_size = { infinity<float>, infinity<float> });
    resolve_constraint(const resolve_constraint& other);
    resolve_constraint& operator=(const resolve_constraint& other);
    resolve_constraint(resolve_constraint&& other);
    resolve_constraint& operator=(resolve_constraint&& other);

    [[nodiscard]] float2& min_size();
    [[nodiscard]] const float2& min_size() const;
    [[nodiscard]] float2& max_size();
    [[nodiscard]] const float2& max_size() const;

    resolve_constraint flip();
    resolve_constraint normalize();
    resolve_constraint enforce(const resolve_constraint& constraint);
    resolve_constraint deflate(const float2& insets);
    resolve_constraint loosen();
    resolve_constraint tighten(const float2& size);

    [[nodiscard]] bool has_bounded_height() const;
    [[nodiscard]] bool has_bounded_width() const;
    [[nodiscard]] bool has_infinite_height() const;
    [[nodiscard]] bool has_infinite_width() const;
    [[nodiscard]] bool has_tight_height() const;
    [[nodiscard]] bool has_tight_width() const;
    [[nodiscard]] bool is_normalized() const;

    [[nodiscard]] float2 constrain(const float2& size) const;
    [[nodiscard]] float2 biggest() const;
    [[nodiscard]] float2 smallest() const;

private:
    detail::resolve_constraint_data _data;
};

struct resolve_command {
    resolve_command(const detail::resolve_command_data& data) { }

    /// @brief
    const resolve_constraint& constraint();

    /// @brief Then
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param constraint
    /// @return
    template <typename child_widget_t>
    float2 constrain_child(child_widget_t& child_widget, const resolve_constraint& constraint);

    template <typename child_widget_t>
    float2 constrain_child_loose(child_widget_t& child_widget, const float2& max_size);

    template <typename child_widget_t>
    float2 constrain_child_tight(child_widget_t& child_widget, const float2& fixed_size);

    template <typename child_widget_t>
    float2 constrain_child_expanded(child_widget_t& child_widget);

    /// @brief Thenafter
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param position
    template <typename child_widget_t>
    void position_child(child_widget_t& child_widget, const float2& position);

private:
    detail::resolve_command_data _data;
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param resolve_callback
template <typename widget_t, typename... children_widgets_t>
void on_resolve(widget_t* widget, const std::function<float2(const resolve_command&, children_widgets_t&...)>& resolve_callback, children_widgets_t&... children_widgets);

/// @brief
/// @details
struct draw_command {
    draw_command(detail::draw_command_data& data);
    draw_command(const draw_command& other);
    draw_command& operator=(const draw_command& other);
    draw_command(draw_command&& other);
    draw_command& operator=(draw_command&& other);

    /// @brief
    /// @param first_point
    /// @param second_point
    /// @param color
    /// @param thickness
    void draw_line(
        const float2& first_point, const float2& second_point,
        const float4& color,
        const float thickness = 1.f);

    /// @brief
    /// @param min_point
    /// @param max_point
    /// @param color
    /// @param rounding
    /// @param thickness
    void draw_rect(
        const float2& min_point, const float2& max_point,
        const float4& color,
        const float rounding_strength = 0.f,
        const float thickness = 1.f);

    /// @brief
    /// @param min_point
    /// @param max_point
    /// @param color
    /// @param rounding
    void draw_rect_filled(
        const float2& min_point, const float2& max_point,
        const float4& color,
        const float rounding = 0.f);

    /// @brief
    /// @param min_corner
    /// @param max_corner
    /// @param color_top_left
    /// @param color_top_right
    /// @param color_bottom_left
    /// @param color_bottom_right
    void draw_rect_filled_multi_color(
        const float2& min_corner, const float2& max_corner,
        const float4& color_top_left, const float4& color_top_right,
        const float4& color_bottom_left, const float4& color_bottom_right);

    /// @brief
    /// @param top_left_corner
    /// @param top_right_corner
    /// @param bottom_left_corner
    /// @param bottom_right_corner
    /// @param color
    /// @param thickness
    void draw_quad(
        const float2& top_left_corner, const float2& top_right_corner,
        const float2& bottom_left_corner, const float2& bottom_right_corner,
        const float4& color,
        const float thickness = 1.f);

    /// @brief
    /// @param top_left_corner
    /// @param top_right_corner
    /// @param bottom_left_corner
    /// @param bottom_right_corner
    /// @param color
    void draw_quad_filled(
        const float2& top_left_corner, const float2& top_right_corner,
        const float2& bottom_left_corner, const float2& bottom_right_corner,
        const float4& color);

    /// @brief
    /// @param first_corner
    /// @param second_corner
    /// @param third_corner
    /// @param color
    /// @param thickness
    void draw_triangle(
        const float2& first_corner, const float2& second_corner, const float2& third_corner,
        const float4& color,
        const float thickness = 1.f);

    /// @brief
    /// @param first_corner
    /// @param second_corner
    /// @param third_corner
    /// @param color
    void draw_triangle_filled(
        const float2& first_corner, const float2& second_corner, const float2& third_corner,
        const float4& color);

    // draw bezier and bspline from points direct (sans curve)

private:
    detail::draw_command_data& _command_data;
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