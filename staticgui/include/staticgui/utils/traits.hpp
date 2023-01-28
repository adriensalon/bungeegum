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

namespace internal {
    namespace traits {

        // add
        template <typename value_t, typename = std::void_t<>>
        inline constexpr bool has_outside_add = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        inline constexpr bool has_outside_add<value_t, std::void_t<decltype(operator+(std::declval<value_t>(), std::declval<value_t>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        inline constexpr bool has_inside_add = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        inline constexpr bool has_inside_add<value_t, std::void_t<decltype(std::declval<value_t>().operator+(std::declval<value_t>()))>> = true;

        // float multiply
        template <typename value_t, typename = std::void_t<>>
        inline constexpr bool has_outside_float_right_multiply = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        inline constexpr bool has_outside_float_right_multiply<value_t, std::void_t<decltype(operator*(std::declval<value_t>(), std::declval<float>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        inline constexpr bool has_outside_float_left_multiply = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        inline constexpr bool has_outside_float_left_multiply<value_t, std::void_t<decltype(operator*(std::declval<float>(), std::declval<value_t>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        inline constexpr bool has_inside_float_multiply = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        inline constexpr bool has_inside_float_multiply<value_t, std::void_t<decltype(std::declval<value_t>().operator*(std::declval<float>()))>> = true;

    }
}

namespace traits {

    template <typename first_value_t, typename... other_values_t>
    struct unique_value {
        using type = first_value_t;
    };

    template <typename value_t>
    inline constexpr bool has_add = (internal::traits::has_outside_add<value_t> || internal::traits::has_inside_add<value_t>);

    template <typename value_t>
    inline constexpr bool has_float_multiply = (internal::traits::has_outside_float_left_multiply<value_t> || internal::traits::has_outside_float_right_multiply<value_t> || internal::traits::has_inside_float_multiply<value_t>);
}
}
