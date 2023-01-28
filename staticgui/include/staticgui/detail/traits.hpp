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
namespace traits {

    template <typename value_t, typename = std::void_t<>>
    inline constexpr bool has_add = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    inline constexpr bool has_add<value_t, std::void_t<decltype(operator+(std::declval<value_t>(), std::declval<value_t>()))>> = true;

    template <typename value_t, typename = std::void_t<>>
    inline constexpr bool has_float_right_multiply = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    inline constexpr bool has_float_right_multiply<value_t, std::void_t<decltype(operator*(std::declval<value_t>(), std::declval<float>()))>> = true;

    template <typename value_t, typename = std::void_t<>>
    inline constexpr bool has_float_left_multiply = std::is_arithmetic_v<value_t>;

    template <typename value_t>
    inline constexpr bool has_float_left_multiply<value_t, std::void_t<decltype(operator*(std::declval<float>(), std::declval<value_t>()))>> = true;

}
}
