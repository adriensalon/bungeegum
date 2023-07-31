#pragma once

#include <bungeegum/core/context.fwd>

namespace bungeegum {

/// @brief
/// @tparam value_t
template <typename value_t>
bool is_contextual(const std::string& name);

/// @brief
/// @tparam value_t
template <typename value_t>
const value_t& get_contextual(const std::string& name);

/// @brief
/// @tparam value_t
template <typename value_t>
void set_contextual(const std::string& name, const value_t& value);

/// @brief
/// @tparam value_t
template <typename value_t>
void set_contextual(const std::string& name, value_t&& value);

}

#include <bungeegum/core/context.inl>