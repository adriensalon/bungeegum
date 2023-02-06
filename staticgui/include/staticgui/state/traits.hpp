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

#include <staticgui/glue/typelist.hpp>

namespace staticgui {
namespace detail {

    template <typename... types_t>
    constexpr bool is_only_type = (glue::typelist<types_t...>().size() == 1);

    template <typename... types_t>
    using first_type = decltype(glue::typelist<types_t...>().front());

    template <typename type_t, typename... types_t>
    constexpr bool is_first_type = (is_only_type<types_t...> && std::is_same_v<type_t, first_type<types_t...>>);

    //
    //
    //
    //
    //
    //
    //

    template <typename... multiple_t>
    struct value_or_tuple {
        using type = std::tuple<multiple_t...>;
    };

    template <typename only_t>
    struct value_or_tuple<only_t> {
        using type = only_t;
    };

    template <>
    struct value_or_tuple<> {
        using type = void;
    };

}
}
