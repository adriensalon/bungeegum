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
        constexpr bool has_outside_add = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_outside_add<value_t, std::void_t<decltype(operator+(std::declval<value_t>(), std::declval<value_t>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_inside_add = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_inside_add<value_t, std::void_t<decltype(std::declval<value_t>().operator+(std::declval<value_t>()))>> = true;

        // float multiply
        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_outside_float_right_multiply = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_outside_float_right_multiply<value_t, std::void_t<decltype(operator*(std::declval<value_t>(), std::declval<float>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_outside_float_left_multiply = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_outside_float_left_multiply<value_t, std::void_t<decltype(operator*(std::declval<float>(), std::declval<value_t>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_inside_float_multiply = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_inside_float_multiply<value_t, std::void_t<decltype(std::declval<value_t>().operator*(std::declval<float>()))>> = true;

        // function (pas mal la syntaxe)

        template <typename value_t, typename = std::void_t<>>
        struct is_callable : std::is_function<value_t> {
        };

        template <typename value_t>
        struct is_callable<value_t, typename std::enable_if_t<std::is_same_v<decltype(void(&value_t::operator())), void>>> : std::true_type {
        };
    }

    namespace traits {

        // template <typename first_value_t, typename... other_values_t>
        // struct first_value_impl {
        //     using type = first_value_t;
        // };

        // template <typename... values_t>
        // struct first_value {
        //     template <typename = typename std::enable_if_t<sizeof...(values_t) >= 1>>
        //     first_value() { }

        //     using type = typename first_value_impl<values_t...>::type;
        // };

        // template <typename value_t>
        // constexpr bool has_add_v = (internal::traits::has_outside_add<value_t> || internal::traits::has_inside_add<value_t>);

        // template <typename value_t>
        // constexpr bool has_float_multiply_v = (internal::traits::has_outside_float_left_multiply<value_t> || internal::traits::has_outside_float_right_multiply<value_t> || internal::traits::has_inside_float_multiply<value_t>);

        // template <typename value_t>
        // constexpr bool is_lerpable = (has_float_multiply<value_t>() && has_add<value_t>());

        template <typename function_t, typename... values_t>
        constexpr bool is_convertible_to_callback()
        {
            return std::conjunction_v<std::negation<std::is_same<std::decay_t<function_t>, std::function<void(values_t...)>>>, std::is_invocable_r<void, function_t, values_t...>>;
            // return std::is_invocable_v<function_t, values_t...>;
        }
    }
}
}