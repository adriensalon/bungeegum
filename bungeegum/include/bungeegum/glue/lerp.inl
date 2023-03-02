//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace bungeegum {
namespace detail {

    template <typename value_t>
    value_t lerp(lerpable_t<value_t>&& min_value, value_t&& max_value, const float t)
    {
        return (1.f - t) * min_value + t * max_value;
    }
}
}
