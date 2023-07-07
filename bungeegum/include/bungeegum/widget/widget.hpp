#pragma once

#include <bungeegum/widget/widget.fwd>

namespace bungeegum {

/// @brief
/// @tparam widget_t
template <typename widget_t>
struct reference_widget {

    /// @brief
    widget_t& get();

    /// @brief
    const widget_t& get() const;

    /// @brief
    widget_t& operator&();

    /// @brief
    const widget_t& operator&() const;

    reference_widget() = delete;
    reference_widget(const reference_widget& other) = default;
    reference_widget& operator=(const reference_widget& other) = default;
    reference_widget(reference_widget&& other) = default;
    reference_widget& operator=(reference_widget&& other) = default;

    reference_widget(detail::reloaded<widget_t>&& reloaded);

private:
    friend struct detail::widgets_registry;
    detail::reloaded<widget_t> _data;
};

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return Returns a reference to the new widget
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] reference_widget<widget_t>& make_reference(widget_args_t&&... widget_args);

template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args)
{
    return make_reference<widget_t>(std::forward<widget_args_t>()...).get();
}
}

#include <bungeegum/widget/widget.inl>
