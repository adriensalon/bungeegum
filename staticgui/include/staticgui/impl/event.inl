//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {
namespace detail {

    inline static glue::registry global_events;
}

template <typename... values_t>
event<values_t...>::event(const std::function<void(values_t...)>& trigger_callback)
// : _impl(
//     detail::global_events.create_component<detail::event_component>(
//         detail::global_events.create_entity()))
{
}

}