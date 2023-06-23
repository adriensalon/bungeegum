#pragma once

#include <bungeegum/context/resolve.fwd>

namespace bungeegum {

struct resolve_command {

    /// @brief
    [[nodiscard]] float2 min_size() const;

    /// @brief
    [[nodiscard]] float2 max_size() const;

    /// @brief
    /// @param size
    void resize(const float2 size);

    /// @brief
    /// @param child_widget
    /// @param constraint
    float2 resolve_child(const runtime_widget& child_widget, const float2 min_size, const float2 max_size) const;

    /// @brief
    /// @param child_widget
    /// @param position
    void position_child(const runtime_widget& child_widget, const float2 position);

private:
    detail::resolve_command_data _data;
    friend struct detail::context;
};

// /// @brief
// /// @tparam widget_t
// /// @param widget
// /// @param resolve_callback
// template <typename widget_t>
// void on_resolve(typed_widget_t<widget_t>& widget, const std::function<void(resolve_command&)>& resolve_callback);

// /// @brief
// /// @tparam widget_t
// /// @param widget
// /// @param resolve_callback
// template <typename widget_t>
// void on_resolve(typed_widget_t<widget_t>* widget, const std::function<void(resolve_command&)>& resolve_callback);

void on_resolve(const runtime_widget& widget, const std::function<void(resolve_command&)>& resolve_callback);

/// @brief
void must_resolve();

// /// @brief
// /// @tparam widget_t
// /// @param widget must have declared a void resolve(const resolve_command&) or added one
// /// dynamically with the on_resolve() function.
// template <typename widget_t>
// void must_resolve(typed_widget_t<widget_t>& widget);

// /// @brief
// /// @tparam widget_t
// /// @param widget must have declared a void resolve(const resolve_command&) or added one
// /// dynamically with the on_resolve() function.
// template <typename widget_t>
// void must_resolve(typed_widget_t<widget_t>* widget);

void must_resolve(const runtime_widget& widget);
}

#include <bungeegum/context/resolve.inl>