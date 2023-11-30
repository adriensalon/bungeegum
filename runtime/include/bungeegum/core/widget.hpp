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

/// @brief Opaque untyped
/// @details Instances of this type can be copied (shallow copy) and moved.
struct widget_id {
    widget_id() = delete;
    widget_id(const widget_id& other) = default;
    widget_id& operator=(const widget_id& other) = default;
    widget_id(widget_id&& other) = default;
    widget_id& operator=(widget_id&& other) = default;

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    widget_id(widget_t* widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    widget_id(widget_t& widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    widget_id(widget_reference<widget_t>& widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    widget_id(widget_ptr<widget_t>& widget);

private:
    widget_id(const detail::widget_id_data& data);

    friend struct detail::widgets_manager;
    detail::widget_id_data _data;
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

/// @brief Destroys a widget created with the make_reference function. References to it will no longer
/// be valid.
void destroy(const widget_id& widget);

/// @brief
void adopt(const widget_id& widget, const widget_id& child_widget);

/// @brief
void abandon(const widget_id& parent_widget, const widget_id& child_widget);

/// @brief
template <template <typename, typename> typename container_t, typename allocator_t = std::allocator<widget_id>>
void get_children(const widget_id& widget, container_t<widget_id, allocator_t>& container);

/// @brief
[[nodiscard]] bool has_parent(const widget_id& widget);

/// @brief
widget_id get_parent(const widget_id& widget);

/// @brief
[[nodiscard]] resolve_command& get_resolve_command(const widget_id& widget);

/// @brief
[[nodiscard]] bool has_interact_command(const widget_id& widget);

/// @brief
[[nodiscard]] interact_command& get_interact_command(const widget_id& widget);

/// @brief
[[nodiscard]] bool has_draw_command(const widget_id& widget);

/// @brief
[[nodiscard]] draw_command& get_draw_command(const widget_id& widget);

/// @brief
template <typename widget_t>
[[nodiscard]] bool has_type(const widget_id& widget);
}

#include <bungeegum/core/widget.inl>
