#pragma once

#include <bungeegum/widget/runtime.hpp>

namespace bungeegum {

// PROPERTY REF COMME CA PROPERTIES HOTRELOADABLES

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
template <typename property_t>
void destroy_property(const runtime_widget& widget, const std::string& name);

}

#include <bungeegum/widget/property.inl>