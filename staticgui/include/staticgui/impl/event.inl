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
#include <typeindex>

#include <staticgui/staticgui.hpp>
#include <staticgui/utils/ecs.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        struct event_impl {
            std::vector<std::type_index> typeinfos;
            std::vector<tools::any_function> callbacks;
            // ecs::entity owner;
        };

        inline static ecs::registry events_container;

        event_impl& create_event_impl();
        void destroy_event_impl(event_impl& event);
    }
}

template <typename... values_t>
event<values_t...>::event(const std::function<void(values_t...)>& trigger_callback)
    : _impl(internal::impl::create_event_impl())
{
}

// template <typename... values_t>
// template <typename function_t = typename std::enable_if_t<is_convertible_to_callback<function_t, values_t...>>>
// event<values_t...>::event(function_t&& function)
//     : _impl(internal::impl::create_event_impl())
// {
//     // static_assert(traits::is_convertible_to_callback<function_t, values_t...>); // we have to rely on static assert
//     std::cout << "YEEEEEEEEES \n";
// }

}