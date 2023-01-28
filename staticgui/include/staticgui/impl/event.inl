//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>

#include <staticgui/staticgui.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        struct runtime_event_component {
            std::vector<std::type_info> typeinfos;
            std::any untyped = nullptr;

            // template <typename... values_t>
            // std::function<void(values_t&...)> trigger = nullptr;

            // template <typename... values_t>
            // std::function<void(std::function<void(values_t&...)>)> on_trigger = nullptr;
        };

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