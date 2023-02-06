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

    template <typename... values_t>
    using future_values = glue::future_typelist_t<values_t...>;

}
}
