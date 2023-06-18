#pragma once

#include <bungeegum/context/resolve.fwd>

namespace bungeegum {

struct adopted_widget;

struct resolve_command {

    /// @brief
    [[nodiscard]] float2 min_size() const;

    /// @brief
    [[nodiscard]] float2 max_size() const;

    /// @brief
    /// @param size
    void resize(const float2 size);

    /// @brief Then
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param constraint
    template <typename child_widget_t>
    float2 resolve_child(const child_widget_t& child_widget, const float2 min_size, const float2 max_size)
    {
        // todo
        (void)child_widget;
        (void)min_size;
        (void)max_size;
        return zero<float2>;
    }

    /// @brief
    /// @param child_widget
    /// @param constraint
    float2 resolve_child(const adopted_widget& child_widget, const float2 min_size, const float2 max_size) const;

    /// @brief Thenafter
    /// @tparam child_widget_t
    /// @param child_widget
    /// @param position
    template <typename child_widget_t>
    void position_child(child_widget_t& child_widget, const float2 position)
    {
        // todo
        (void)child_widget;
        (void)position;
    }

    /// @brief
    /// @param child_widget
    /// @param position
    void position_child(const adopted_widget& child_widget, const float2 position);

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