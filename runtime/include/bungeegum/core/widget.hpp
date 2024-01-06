#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief
/// @details Instances of this type can be copied (shallow copy) and moved.
/// @tparam widget_t
template <typename widget_t>
struct widget_ref {
    widget_ref() = delete;
    widget_ref(const widget_ref& other) = default;
    widget_ref& operator=(const widget_ref& other) = default;
    widget_ref(widget_ref&& other) = default;
    widget_ref& operator=(widget_ref&& other) = default;

    /// @brief
    widget_t& get();

    /// @brief
    widget_t* operator->();

private:
	friend struct detail::widget_ref_access<widget_t>;
    widget_ref(const detail::widget_ref_data<widget_t>& data);
    detail::widget_ref_data<widget_t> _data;
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
    widget_id(widget_ref<widget_t>& widget);

private:
	friend struct detail::widget_id_access;
    widget_id(const detail::widget_id_data& data);
    detail::widget_id_data _data;
};

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @return Returns a reference to the new widget
template <typename widget_t>
[[nodiscard]] widget_ref<widget_t> make_ref();

/// @brief
/// @tparam widget_t
template <typename widget_t>
[[nodiscard]] widget_t& make();

/// @brief Destroys a widget created with the make_ref function. References to it will no longer
/// be valid.
void destroy(const widget_id& widget);

/// @brief
void adopt(const widget_id& widget, const widget_id& child_widget);

/// @brief
void abandon(const widget_id& parent_widget, const widget_id& child_widget);

/// @brief 
/// @param widget 
/// @return 
[[nodiscard]] std::vector<widget_id> get_children(const widget_id& widget);

/// @brief
[[nodiscard]] bool has_parent(const widget_id& widget);

/// @brief
[[nodiscard]] widget_id get_parent(const widget_id& widget);

/// @brief 
/// @tparam widget_t 
/// @param widget 
/// @return 
template <typename widget_t>
[[nodiscard]] widget_ref<widget_t> get_ref(const widget_id& widget);

/// @brief 
/// @tparam widget_t 
/// @param widget 
/// @return 
template <typename widget_t>
[[nodiscard]] bool has_type(const widget_id& widget);

/// @brief 
/// @param root_widget 
/// @param callback 
void traverse(const widget_id& root_widget, const std::function<void(const widget_id& widget)>& callback);
}

#include <bungeegum/core/widget.inl>
