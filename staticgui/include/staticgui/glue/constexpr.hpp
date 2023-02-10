//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/typelist.hpp>

namespace staticgui {
namespace detail {

    template <unsigned int start_t, unsigned int end_t, unsigned int increment_t, typename function_t>
    constexpr void constexpr_for(function_t&& function);

    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, values_t&... values);

}
}

#include <staticgui/glue/constexpr.inl>
