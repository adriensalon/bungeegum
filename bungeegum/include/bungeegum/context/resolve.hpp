#pragma once

#include <bungeegum/context/resolve.fwd>

namespace bungeegum {

/// @brief
/// @details We mimick the Flutter RenderBox layout model : https://api.flutter.dev/flutter/rendering/BoxConstraints-class.html
struct resolve_constraint {

    /// @brief
    [[nodiscard]] float2& min_size();

    /// @brief
    [[nodiscard]] const float2 min_size() const;

    /// @brief
    [[nodiscard]] float2& max_size();

    /// @brief
    [[nodiscard]] const float2 max_size() const;

    /// @brief
    void flip();

    /// @brief
    void normalize();

    /// @brief
    /// @param constraint
    void enforce(const resolve_constraint& constraint);

    /// @brief
    /// @param insets
    void deflate(const float2 insets);

    /// @brief
    void loosen();

    /// @brief
    /// @param size
    void tighten(const float2 size);

    /// @brief
    [[nodiscard]] bool has_bounded_height() const;

    /// @brief
    [[nodiscard]] bool has_bounded_width() const;

    /// @brief
    [[nodiscard]] bool has_infinite_height() const;

    /// @brief
    [[nodiscard]] bool has_infinite_width() const;

    /// @brief
    [[nodiscard]] bool has_tight_height() const;

    /// @brief
    [[nodiscard]] bool has_tight_width() const;

    /// @brief
    [[nodiscard]] bool is_normalized() const;

    /// @brief
    /// @param size
    [[nodiscard]] float2 constrain(const float2 size) const;

    /// @brief
    [[nodiscard]] float2 biggest() const;

    /// @brief
    [[nodiscard]] float2 smallest() const;

private:
    detail::resolve_constraint_data _data;
    friend struct detail::context;
};

struct resolve_command {

    /// @brief
    [[nodiscard]] const resolve_constraint& constraint();

    /// @brief
    /// @param size
    void resize(const float2 size);

    /// @brief Then
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param constraint
    template <typename child_widget_t>
    float2 resolve_child(child_widget_t& child_widget, const resolve_constraint& constraint);

    /// @brief
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param max_size
    template <typename child_widget_t>
    float2 resolve_child_loose(child_widget_t& child_widget, const float2 max_size);

    /// @brief
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param fixed_size
    template <typename child_widget_t>
    float2 resolve_child_tight(child_widget_t& child_widget, const float2 fixed_size);

    /// @brief
    /// @tparam child_widget_t
    /// @param child_widget
    template <typename child_widget_t>
    float2 resolve_child_expanded(child_widget_t& child_widget);

    /// @brief Thenafter
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param position
    template <typename child_widget_t>
    void position_child(child_widget_t& child_widget, const float2 position);

private:
    detail::resolve_command_data _data;
    friend struct detail::context;
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param resolve_callback
template <typename widget_t>
void on_resolve(widget_t& widget, const std::function<void(resolve_command&)>& resolve_callback);

/// @brief
/// @tparam widget_t
/// @param widget
/// @param resolve_callback
template <typename widget_t>
void on_resolve(widget_t* widget, const std::function<void(resolve_command&)>& resolve_callback);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void must_resolve(widget_t& widget);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void must_resolve(widget_t* widget);

}

#include <bungeegum/context/resolve.inl>