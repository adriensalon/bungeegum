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
