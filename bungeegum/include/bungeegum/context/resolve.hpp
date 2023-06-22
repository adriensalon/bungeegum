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

    resolve_command& child_command(const runtime_widget& child_widget) const;

    template <typename value_t>
    std::optional<value_t>& properties(const std::string& name);

    /// @brief Then
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param constraint
    template <typename child_widget_t>
    float2 resolve_child(const typed_widget_t<child_widget_t>& child_widget, const float2 min_size, const float2 max_size);

    /// @brief
    /// @param child_widget
    /// @param constraint
    float2 resolve_child(const runtime_widget& child_widget, const float2 min_size, const float2 max_size) const;

    /// @brief Thenafter
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param position
    template <typename child_widget_t>
    void position_child(typed_widget_t<child_widget_t>& child_widget, const float2 position);

    /// @brief
    /// @param child_widget
    /// @param position
    void position_child(const runtime_widget& child_widget, const float2 position);

private:
    detail::resolve_command_data _data;
    friend struct detail::context;
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param resolve_callback
template <typename widget_t>
void on_resolve(typed_widget_t<widget_t>& widget, const std::function<void(resolve_command&)>& resolve_callback);

/// @brief
/// @tparam widget_t
/// @param widget
/// @param resolve_callback
template <typename widget_t>
void on_resolve(typed_widget_t<widget_t>* widget, const std::function<void(resolve_command&)>& resolve_callback);

/// @brief
void must_resolve();

/// @brief
/// @tparam widget_t
/// @param widget must have declared a void resolve(const resolve_command&) or added one
/// dynamically with the on_resolve() function.
template <typename widget_t>
void must_resolve(typed_widget_t<widget_t>& widget);

/// @brief
/// @tparam widget_t
/// @param widget must have declared a void resolve(const resolve_command&) or added one
/// dynamically with the on_resolve() function.
template <typename widget_t>
void must_resolve(typed_widget_t<widget_t>* widget);
}

#include <bungeegum/context/resolve.inl>