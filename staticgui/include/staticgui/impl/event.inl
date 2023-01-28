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
    }
}

// template <typename... values_t>
// event<values_t...>::event(std::function<void(const values_t&...)> trigger_callback)
// {
//     std::cout << "YES \n";
// }

}