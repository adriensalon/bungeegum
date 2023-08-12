#pragma once

#include <type_traits>

namespace bungeegum {

namespace detail {

    // type has addition
    template <typename value_t, typename = std::void_t<>>
    constexpr bool has_outside_add_v = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    constexpr bool has_outside_add_v<value_t, std::void_t<decltype(operator+(std::declval<value_t>(), std::declval<value_t>()))>> = true;

    template <typename value_t, typename = std::void_t<>>
    constexpr bool has_inside_add_v = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    constexpr bool has_inside_add_v<value_t, std::void_t<decltype(std::declval<value_t>().operator+(std::declval<value_t>()))>> = true;

    template <typename value_t>
    constexpr bool has_add_v = (has_outside_add_v<value_t> || has_inside_add_v<value_t>);

    // type has float multiplication
    template <typename value_t, typename = std::void_t<>>
    constexpr bool has_outside_float_right_multiply_v = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    constexpr bool has_outside_float_right_multiply_v<value_t, std::void_t<decltype(operator*(std::declval<value_t>(), std::declval<float>()))>> = true;

    template <typename value_t, typename = std::void_t<>>
    constexpr bool has_outside_float_left_multiply_v = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    constexpr bool has_outside_float_left_multiply_v<value_t, std::void_t<decltype(operator*(std::declval<float>(), std::declval<value_t>()))>> = true;

    template <typename value_t, typename = std::void_t<>>
    constexpr bool has_inside_float_multiply_v = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    constexpr bool has_inside_float_multiply_v<value_t, std::void_t<decltype(std::declval<value_t>().operator*(std::declval<float>()))>> = true;

    template <typename value_t>
    constexpr bool has_float_multiply_v = (has_outside_float_left_multiply_v<value_t> || has_outside_float_right_multiply_v<value_t> || has_inside_float_multiply_v<value_t>);

    // type has both
    template <typename value_t>
    constexpr bool is_lerpable_v = (detail::has_float_multiply_v<value_t> && detail::has_add_v<value_t>);

    template <typename value_t>
    using lerpable_t = typename std::enable_if_t<is_lerpable_v<value_t>, value_t>;
}

template <typename value_t>
detail::lerpable_t<value_t> lerp(const value_t& min_value, const value_t& max_value, const float t)
{
    static_assert(detail::has_add_v<value_t>, "This type is not lerpable because it is not arithmetic and doesn't "
                                              "implement the operator+(const value_t&) method.");
    static_assert(detail::has_float_multiply_v<value_t>, "This type is not lerpable because it is not arithmetic and doesn't "
                                                         "implement the operator*(const float&) method.");
    return (min_value * (1.f - t) + max_value * t);
}
}
