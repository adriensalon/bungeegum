//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/staticgui.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        struct event_impl {
        };

        static internal::impl::event_impl _NO5;
    }
}

template <typename... values_t>
template <typename function_t>
event<values_t...>::event(function_t&& function)
    : _impl(internal::impl::_NO5)
{
    static_assert(std::is_invocable_v<function_t, values_t...>); // we have to rely on static assert
    std::cout << "YEEEEEEEEES \n";
}

}