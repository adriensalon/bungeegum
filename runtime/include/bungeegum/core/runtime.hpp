#pragma once

#include <bungeegum/core/runtime.fwd>

namespace bungeegum {

/// @brief Opaque untyped
/// @details Instances of this type can be copied (shallow copy) and moved.
struct runtime_widget {
    runtime_widget() = delete;
    runtime_widget(const runtime_widget& other) = default;
    runtime_widget& operator=(const runtime_widget& other) = default;
    runtime_widget(runtime_widget&& other) = default;
    runtime_widget& operator=(runtime_widget&& other) = default;

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    runtime_widget(widget_t* widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    runtime_widget(widget_t& widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    runtime_widget(widget_reference<widget_t>& widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    runtime_widget(widget_ptr<widget_t>& widget);

private:
    runtime_widget(const detail::runtime_widget_data& data);

    friend struct detail::widgets_manager;
    detail::runtime_widget_data _data;
};

/// @brief Destroys a widget created with the make_reference function. References to it will no longer
/// be valid.
void destroy(const runtime_widget& widget);

/// @brief
void adopt(const runtime_widget& widget, const runtime_widget& child_widget);

/// @brief
void abandon(const runtime_widget& parent_widget, const runtime_widget& child_widget);

/// @brief
template <template <typename, typename> typename container_t, typename allocator_t = std::allocator<runtime_widget>>
void get_children(const runtime_widget& widget, container_t<runtime_widget, allocator_t>& container);

/// @brief
[[nodiscard]] bool has_parent(const runtime_widget& widget);

/// @brief
runtime_widget get_parent(const runtime_widget& widget);

/// @brief
[[nodiscard]] resolve_command& get_resolve_command(const runtime_widget& widget);

/// @brief
[[nodiscard]] bool has_interact_command(const runtime_widget& widget);

/// @brief
[[nodiscard]] interact_command& get_interact_command(const runtime_widget& widget);

/// @brief
[[nodiscard]] bool has_draw_command(const runtime_widget& widget);

/// @brief
[[nodiscard]] draw_command& get_draw_command(const runtime_widget& widget);

/// @brief
template <typename widget_t>
[[nodiscard]] bool has_type(const runtime_widget& widget);

}

#include <bungeegum/core/runtime.inl>