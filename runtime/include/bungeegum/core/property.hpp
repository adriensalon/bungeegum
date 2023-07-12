#pragma once

#include <bungeegum/core/runtime.hpp>

namespace bungeegum {

/// @brief
/// @tparam widget_t
template <typename widget_t>
struct property_reference {
    property_reference() = delete;
    property_reference(const property_reference& other) = default;
    property_reference& operator=(const property_reference& other) = default;
    property_reference(property_reference&& other) = default;
    property_reference& operator=(property_reference&& other) = default;

    /// @brief
    widget_t& get() const;

    /// @brief
    widget_t* operator->() const;

private:
    property_reference(detail::value_type_t<widget_t>& data);

    friend struct detail::widgets_manager;
    detail::reference_type_t<widget_t> _data;
};

/// @brief
template <typename property_t>
[[nodiscard]] property_reference<property_t> make_property_reference(const runtime_widget& widget);

/// @brief
template <typename property_t>
[[nodiscard]] property_t& make_property(const runtime_widget& widget);

/// @brief
template <typename property_t>
[[nodiscard]] bool has_property(const runtime_widget& widget);

/// @brief
template <typename property_t>
[[nodiscard]] property_t& get_property(const runtime_widget& widget);

/// @brief
template <typename property_t>
[[nodiscard]] property_t& get_or_make_property(const runtime_widget& widget);

/// @brief
template <typename property_t>
void destroy_property(const runtime_widget& widget);
}

#include <bungeegum/core/property.inl>