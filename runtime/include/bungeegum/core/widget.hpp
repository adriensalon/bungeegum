#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief
/// @details Instances of this type can be copied (shallow copy) and moved.
/// @tparam widget_t
template <typename widget_t>
struct widget_reference {
    widget_reference() = delete;
    widget_reference(const widget_reference& other) = default;
    widget_reference& operator=(const widget_reference& other) = default;
    widget_reference(widget_reference&& other) = default;
    widget_reference& operator=(widget_reference&& other) = default;

    /// @brief
    widget_t& get() const;

    /// @brief
    widget_t* operator->() const;

private:
    widget_reference(detail::value_type_t<widget_t>& data);

    friend struct access;
    friend struct detail::widgets_manager;
    detail::reference_type_t<widget_t> _data;
};

/// @brief
/// @details Instances of this type can be copied (shallow copy) and moved.
/// @tparam widget_t
template <typename widget_t>
struct widget_ptr {
    widget_ptr() = default;
    widget_ptr(const widget_ptr& other) = default;
    widget_ptr& operator=(const widget_ptr& other) = default;
    widget_ptr(widget_ptr&& other) = default;
    widget_ptr& operator=(widget_ptr&& other) = default;

    /// @brief
    widget_t* get() const;

    /// @brief
    widget_t* operator->() const;

private:
    widget_ptr(detail::value_type_t<widget_t>& data);

    friend struct access;
    friend struct detail::widgets_manager;
    detail::reference_type_t<widget_t> _data;
};

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @return Returns a reference to the new widget
template <typename widget_t>
[[nodiscard]] widget_reference<widget_t> make_reference();

/// @brief
template <typename widget_t>
[[nodiscard]] widget_ptr<widget_t> make_ptr();

/// @brief
/// @tparam widget_t
template <typename widget_t>
[[nodiscard]] widget_t& make();
}

#include <bungeegum/core/widget.inl>
