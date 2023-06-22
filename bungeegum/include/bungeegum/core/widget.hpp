#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief Opaque untyped
struct runtime_widget {

    // register if needed
    template <typename widget_t>
    runtime_widget(widget_t* widget)
    {
    }

    // register if needed
    template <typename widget_t>
    runtime_widget(widget_t& widget)
    {
    }

private:
    inline runtime_widget() { }
    friend struct interact_command;
    friend struct resolve_command;
    friend struct draw_command;
    friend struct detail::widgets_registry;
    detail::adopted_widget_data _data;
};

template <typename property_t>
struct runtime_property {

    runtime_property(const runtime_widget& widget, const std::string& name, const property_t& property)
    {
    }

    runtime_property(const runtime_widget& widget, const std::string& name, property_t&& property)
    {
    }
};

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return Returns a reference to the new widget
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args);

/// @brief Destroys a widget created with the make function. References to it will no longer
/// be valid.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @param widget
template <typename widget_t>
void unmake(widget_t& widget);

/// @brief Registers a widget as a child of another widget.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @tparam child_widget_t Custom type for the child widget. Must be at least copy or move
/// constructible.
/// @param widget
/// @param child_widget
/// @return
template <typename widget_t, typename child_widget_t>
runtime_widget adopt(widget_t& widget, child_widget_t& child_widget);

/// @brief
/// @tparam widget_t
/// @tparam ...children_widgets_t
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename child_widget_t>
runtime_widget adopt(widget_t* widget, child_widget_t& child_widget);

/// @brief Unregisters a parenting relationship
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void abandon(widget_t& widget, children_widgets_t&... children_widgets);

/// @brief
/// @tparam widget_t
/// @tparam ...children_widgets_t
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void abandon(widget_t* widget, children_widgets_t&... children_widgets);

// register if needed
template <typename widget_t>
std::vector<runtime_widget> children(const typed_widget_t<widget_t>* widget);

// register if needed
template <typename widget_t>
std::vector<runtime_widget> children(const typed_widget_t<widget_t>& widget);

std::vector<runtime_widget> children(const runtime_widget& widget);

// register if needed
template <typename widget_t>
std::optional<runtime_widget> parent(const typed_widget_t<widget_t>* widget);

// register if needed
template <typename widget_t>
std::optional<runtime_widget> parent(const typed_widget_t<widget_t> widget);

std::optional<runtime_widget> parent(const runtime_widget& widget);

// register if needed
template <typename widget_t, typename property_t>
std::optional<property_t&> property(const typed_widget_t<widget_t>* widget, const std::string& name);

// register if needed
template <typename widget_t, typename property_t>
std::optional<property_t&> property(const typed_widget_t<widget_t>& widget, const std::string& name);

template <typename property_t>
std::optional<property_t&> property(const runtime_widget& widget, const std::string& name);
}

#include <bungeegum/core/widget.inl>
