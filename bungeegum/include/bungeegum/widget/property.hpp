#pragma once

#include <bungeegum/widget/runtime.hpp>

namespace bungeegum {

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

}

#include <bungeegum/widget/property.inl>