#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief
/// @details Instances of this type can be copied (shallow copy) and moved.
/// @tparam widget_t
template <typename widget_t>
struct reference_widget {
    reference_widget() = delete;
    reference_widget(const reference_widget& other) = default;
    reference_widget& operator=(const reference_widget& other) = default;
    reference_widget(reference_widget&& other) = default;
    reference_widget& operator=(reference_widget&& other) = default;

    /// @brief
    widget_t& get() const;

    /// @brief
    // const widget_t& get() const;

    /// @brief
    widget_t* operator->() const;

    /// @brief
    // const widget_t* operator->() const;

private:
    friend struct detail::widgets_manager;
    detail::reference_type_t<widget_t> _data;

    reference_widget(detail::value_type_t<widget_t>& reference);
};

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @return Returns a reference to the new widget
template <typename widget_t>
[[nodiscard]] reference_widget<widget_t> make_reference();

/// @brief
/// @tparam widget_t
template <typename widget_t>
[[nodiscard]] widget_t& make();
}

#include <bungeegum/core/widget.inl>
