#pragma once

#include <cstddef>

namespace bungeegum {
namespace detail {    

    /// @brief 
    /// @tparam ...types_t 
    template <typename... types_t>
    struct variadic_reduce {
        constexpr static bool is_void() { return false; }
        constexpr static bool is_value() { return false; }
        constexpr static bool is_tuple() { return true; }
        using type = std::tuple<types_t...>;
    };
    
    template <typename type_t>
    struct variadic_reduce<type_t> {
        constexpr static bool is_void() { return false; }
        constexpr static bool is_value() { return true; }
        constexpr static bool is_tuple() { return false; }
        using type = type_t;
    };

    template <>
    struct variadic_reduce<> {
        constexpr static bool is_void() { return true; }
        constexpr static bool is_value() { return false; }
        constexpr static bool is_tuple() { return false; }
        using type = void;
    };

    /// @brief 
    /// @tparam ...types_t 
    template <typename... types_t>
    using variadic_reduce_t = typename variadic_reduce<types_t...>::type;

    /// @brief Iterates through integers at compile time and calls a functor on each iteration
    /// @tparam function_t Functor type to invoke on each iteration, taking an integer as parameter
    /// representing the current index
    /// @tparam start_t Start index in a standard for loop
    /// @tparam end_t is End not equal index in a standard for loop
    /// @tparam increment_t Increment value in a standard for loop
    template <std::size_t start_t, std::size_t end_t, std::size_t increment_t, typename function_t>
    constexpr void constexpr_for(function_t&& function);

    /// @brief Iterates through typed values, similarly to a fold expression
    /// @tparam ...values_t Typed values to iterate through
    /// @tparam function_t Functor type to invoke on each iteration, taking an auto parameter
    /// representing the current value
    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, values_t&... values);

}
}

#include <bungeegum/glue/foreach.inl>
