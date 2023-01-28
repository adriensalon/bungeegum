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

        struct animation_impl {
        };
    }
}

template <typename... values_t>
animation<values_t...>::animation(const curve<values_t>&... curves)
{
}

template <typename... values_t>
animation<values_t...>& animation<values_t...>::on_value_changed(std::function<void(const values_t&...)> value_changed_callback)
{
}

template <typename... values_t>
template <typename... other_values_t>
animation<values_t...>& animation<values_t...>::start_after(const animation<other_values_t...>& previous)
{
}

template <typename... values_t>
void animation<values_t...>::start()
{
}

template <typename... values_t>
void animation<values_t...>::stop()
{
}

}