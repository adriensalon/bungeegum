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

#include <bungeegum/glue/simd.hpp>

namespace bungeegum {
namespace detail {

    struct resolve_constraint_data {
        float2 min_size;
        float2 max_size;
    };

    struct resolve_command_data {
        resolve_constraint_data constraint; // given by parent
        float2 resolved_size; // set by widget after children
        float2 resolved_position; // given by parent
    };

}

/// @brief
/// @details https://api.flutter.dev/flutter/rendering/BoxConstraints-class.html
struct resolve_constraint {
    resolve_constraint(const float2 min_size = { 0.f, 0.f }, const float2 max_size = { infinity<float>, infinity<float> });
    resolve_constraint(const resolve_constraint& other);
    resolve_constraint& operator=(const resolve_constraint& other);
    resolve_constraint(resolve_constraint&& other);
    resolve_constraint& operator=(resolve_constraint&& other);

    [[nodiscard]] float2& min_size();
    [[nodiscard]] const float2 min_size() const;
    [[nodiscard]] float2& max_size();
    [[nodiscard]] const float2 max_size() const;

    resolve_constraint flip();
    resolve_constraint normalize();
    resolve_constraint enforce(const resolve_constraint& constraint);
    resolve_constraint deflate(const float2 insets);
    resolve_constraint loosen();
    resolve_constraint tighten(const float2 size);

    [[nodiscard]] bool has_bounded_height() const;
    [[nodiscard]] bool has_bounded_width() const;
    [[nodiscard]] bool has_infinite_height() const;
    [[nodiscard]] bool has_infinite_width() const;
    [[nodiscard]] bool has_tight_height() const;
    [[nodiscard]] bool has_tight_width() const;
    [[nodiscard]] bool is_normalized() const;

    [[nodiscard]] float2 constrain(const float2 size) const;
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
    float2 constrain_child_loose(child_widget_t& child_widget, const float2 max_size);

    template <typename child_widget_t>
    float2 constrain_child_tight(child_widget_t& child_widget, const float2 fixed_size);

    template <typename child_widget_t>
    float2 constrain_child_expanded(child_widget_t& child_widget);

    /// @brief Thenafter
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param position
    template <typename child_widget_t>
    void position_child(child_widget_t& child_widget, const float2 position);

private:
    detail::resolve_command_data _data;
};

/// @brief
/// @tparam widget_t
/// @tparam ...children_widgets_t
/// @param widget
/// @param resolve_callback
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void on_resolve(
    widget_t* widget,
    const std::function<float2(resolve_command&, children_widgets_t&...)>& resolve_callback,
    children_widgets_t&... children_widgets);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void must_resolve(widget_t* widget);

}
