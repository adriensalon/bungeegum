#pragma once

#include <bungeegum/core/widget.fwd>

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

/// @brief Opaque untyped
struct runtime_widget {

    template <typename widget_t>
    runtime_widget(widget_t* widget);

    template <typename widget_t>
    runtime_widget(widget_t& widget);

    runtime_widget(const runtime_widget& other) = default;
    runtime_widget& operator=(const runtime_widget& other) = default;
    runtime_widget(runtime_widget&& other) = default;
    runtime_widget& operator=(runtime_widget&& other) = default;

private:
    friend struct detail::widgets_registry;
    detail::runtime_widget_data _data;
    runtime_widget();
};

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return Returns a reference to the new widget
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] reference_widget<widget_t>& make(widget_args_t&&... widget_args);

template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make_and_get(widget_args_t&&... widget_args)
{
    return make<widget_t>(std::forward<widget_args_t>()...).get();
}

/// @brief Destroys a widget created with the make function. References to it will no longer
/// be valid.
void destroy(const runtime_widget& widget);

/// @brief
void adopt(const runtime_widget& widget, const runtime_widget& child_widget);

/// @brief
void abandon(const runtime_widget& parent_widget, const runtime_widget& child_widget);

/// @brief
template <template <typename, typename> typename container_t, typename allocator_t = std::allocator<runtime_widget>>
void get_children(
    const runtime_widget& widget,
    container_t<runtime_widget, allocator_t>& container);

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

// children with type

// parent with type

/// @brief
template <typename property_t>
[[nodiscard]] property_t& make_property(const runtime_widget& widget, const std::string& name);

/// @brief
template <typename property_t>
[[nodiscard]] bool has_property(const runtime_widget& widget, const std::string& name);

/// @brief
template <typename property_t>
[[nodiscard]] property_t& get_property(const runtime_widget& widget, const std::string& name);

/// @brief
template <typename property_t>
[[nodiscard]] property_t& get_or_make_property(const runtime_widget& widget, const std::string& name);

/// @brief
template <typename property_t, template <typename, typename> typename container_t, typename allocator_t = std::allocator<runtime_widget>>
void get_children_with_property(
    const runtime_widget& widget,
    const std::string& name,
    container_t<runtime_widget, allocator_t>& container,
    const bool recursive = false);

/// @brief
template <typename property_t>
[[nodiscard]] bool has_parent_with_property(const runtime_widget& widget, const std::string& name,
    const unsigned int depth = infinity<unsigned int>);

/// @brief
template <typename property_t>
[[nodiscard]] runtime_widget get_parent_with_property(
    const runtime_widget& widget,
    const std::string& name,
    const unsigned int depth = infinity<unsigned int>);

/// @brief
template <typename property_t>
void destroy_property(const runtime_widget& widget, const std::string& name);

// traverse children ?
}

#include <bungeegum/core/widget.inl>
