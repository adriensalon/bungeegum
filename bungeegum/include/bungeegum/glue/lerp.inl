#pragma once

namespace bungeegum {

template <typename value_t>
value_t lerp(detail::lerpable_t<value_t>& min_value, value_t& max_value, const float t)
{
    return min_value * (1.f - t) + max_value * t;
}

// template <typename value_t>
// value_t lerp(value_t& min_value, value_t& max_value, const float t)
// {
//     return min_value * (1.f - t) + max_value * t;
// }
}
