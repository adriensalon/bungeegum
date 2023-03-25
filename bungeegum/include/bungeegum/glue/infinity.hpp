#pragma once

#include <limits>

namespace bungeegum {

template <typename value_t>
constexpr value_t infinity = std::numeric_limits<value_t>::max();

}
