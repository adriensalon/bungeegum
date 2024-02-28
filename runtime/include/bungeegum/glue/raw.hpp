#pragma once

#include <cstdint>

namespace bungeegum {
namespace detail {

    /// @brief Casts a pointer to an unsigned integer while avoiding undefined behavior
    /// @tparam value_t Type to cast from
    template <typename value_t>
    [[nodiscard]] constexpr std::uintptr_t raw_cast(value_t* value);

    /// @brief Casts a reference to an unsigned integer while avoiding undefined behavior
    /// @tparam value_t Type to cast from
    template <typename value_t>
    [[nodiscard]] constexpr std::uintptr_t raw_cast(value_t& value);

    /// @brief Casts an unsigned integer to a reference while avoiding undefined behavior
    /// @tparam value_t Type to cast to
    template <typename value_t>
    [[nodiscard]] constexpr value_t& raw_cast(const std::uintptr_t raw_value);
}
}

#include <bungeegum/glue/raw.inl>