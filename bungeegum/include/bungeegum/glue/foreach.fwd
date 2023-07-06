#pragma once

#include <cstddef>

namespace bungeegum {
namespace detail {

    /// @brief Iterates through integers at compile time and calls a functor on each iteration.
    /// @tparam function_t is the functor type to invoke on each iteration, taking an integer
    /// as parameter representing the current index.
    /// @tparam start_t is like the start index in a standard for loop.
    /// @tparam end_t is like the end not equal index in a standard for loop.
    /// @tparam increment_t is like the increment value in a standard for loop.
    template <std::size_t start_t, std::size_t end_t, std::size_t increment_t, typename function_t>
    constexpr void constexpr_for(function_t&& function);

    /// @brief Iterates through typed values, similarly to a fold expression.
    /// @tparam ...values_t are the typed values to iterate through.
    /// @tparam function_t is the functor type to invoke on each iteration, taking an auto
    /// parameter representing the current value.
    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, values_t&... values);

}
}

#include <bungeegum/glue/foreach.inl>
