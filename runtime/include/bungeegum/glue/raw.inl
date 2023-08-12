#pragma once

namespace bungeegum {
namespace detail {

    template <typename value_t>
    constexpr std::uintptr_t raw_cast(value_t* value)
    {
        return reinterpret_cast<std::uintptr_t>(static_cast<void*>(value));
    }

    template <typename value_t>
    constexpr std::uintptr_t raw_cast(value_t& value)
    {
        return reinterpret_cast<std::uintptr_t>(static_cast<void*>(std::addressof(value)));
    }

    template <typename value_t>
    constexpr value_t& raw_cast(const std::uintptr_t raw_value)
    {
        return *(static_cast<value_t*>(reinterpret_cast<void*>(raw_value)));
    }
}
}