//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <type_traits>

namespace staticgui {
namespace glue {
    namespace traits {

        // add
        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_outside_add_v = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_outside_add_v<value_t, std::void_t<decltype(operator+(std::declval<value_t>(), std::declval<value_t>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_inside_add_v = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_inside_add_v<value_t, std::void_t<decltype(std::declval<value_t>().operator+(std::declval<value_t>()))>> = true;

        template <typename value_t>
        constexpr bool has_add_v = (internal::traits::has_outside_add<value_t> || internal::traits::has_inside_add<value_t>);

        // float multiply
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
        constexpr bool has_float_multiply_v = (has_outside_float_left_multiply<value_t> || has_outside_float_right_multiply<value_t> || has_inside_float_multiply<value_t>);

        // lerpable
        template <typename value_t>
        constexpr bool is_lerpable_v = (has_float_multiply_v<value_t> && has_add_v<value_t>);

    }
}
}