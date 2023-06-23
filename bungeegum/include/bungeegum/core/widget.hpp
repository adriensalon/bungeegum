#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief Opaque untyped
struct runtime_widget {

    template <typename widget_t>
    runtime_widget(widget_t* widget);

    template <typename widget_t>
    runtime_widget(widget_t& widget);

private:
    friend struct resolve_command;
    friend struct detail::widgets_registry;
    detail::adopted_widget_data _data;
    friend void adopt(const runtime_widget& widget, const runtime_widget& child_widget);
    friend void abandon(const runtime_widget& widget, const runtime_widget& child_widget);
    friend resolve_command& get_resolve_command(const runtime_widget& widget);
    friend void must_resolve(const runtime_widget& widget);
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
void destroy(const runtime_widget& widget);

inline void adopt(const runtime_widget& widget, const runtime_widget& child_widget)
{
    detail::untyped_widget_data& _widget_data = widget._data.untyped_widget.value().get();
    detail::untyped_widget_data& _child_widget_data = child_widget._data.untyped_widget.value().get();
    _child_widget_data.parent = _widget_data;
    _widget_data.children.emplace_back(_child_widget_data);
}

inline void abandon(const runtime_widget& parent_widget, const runtime_widget& child_widget)
{
    detail::untyped_widget_data& _widget_data = parent_widget._data.untyped_widget.value().get();
    detail::untyped_widget_data& _child_widget_data = child_widget._data.untyped_widget.value().get();
    _child_widget_data.parent = std::nullopt;
    (void)_widget_data;
    // TODO HEIN
    // _widget_data.children.erase(_child_widget_data);
}

/// @brief
template <template <typename, typename> typename container_t, typename allocator_t = std::allocator<runtime_widget>>
void get_children(const runtime_widget& widget, container_t<runtime_widget, allocator_t>& container)
{
}

bool has_parent(const runtime_widget& widget);

runtime_widget get_parent(const runtime_widget& widget);

inline resolve_command& get_resolve_command(const runtime_widget& widget)
{
    detail::untyped_widget_data& _widget_data = widget._data.untyped_widget.value().get();
    return _widget_data.resolver_command.value();
}

bool has_interact_command(const runtime_widget& widget);

interact_command& get_interact_command(const runtime_widget& widget);

bool has_draw_command(const runtime_widget& widget);

draw_command& get_draw_command(const runtime_widget& widget);

template <typename property_t>
property_t& make_property(const runtime_widget& widget, const std::string& name);

template <typename property_t>
bool has_property(const runtime_widget& widget, const std::string& name);

template <typename property_t>
property_t& get_property(const runtime_widget& widget, const std::string& name);

template <typename property_t>
property_t& get_or_make_property(const runtime_widget& widget, const std::string& name);

template <typename property_t, template <typename, typename> typename container_t, typename allocator_t = std::allocator<runtime_widget>>
void get_children_with_property(
    const runtime_widget& widget,
    const std::string& name,
    container_t<runtime_widget, allocator_t>& container,
    const bool recursive = false)
{
}

template <typename property_t>
bool has_parent_with_property(const runtime_widget& widget, const std::string& name,
    const unsigned int depth = infinity<unsigned int>);

template <typename property_t>
runtime_widget get_parent_with_property(
    const runtime_widget& widget,
    const std::string& name,
    const unsigned int depth = infinity<unsigned int>);

template <typename property_t>
void destroy_property(const runtime_widget& widget, const std::string& name);

// traverse children ?
}

#include <bungeegum/core/widget.inl>
