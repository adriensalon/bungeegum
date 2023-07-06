#pragma once

#include <variant>

namespace bungeegum {
namespace detail {

    template <std::size_t start_t, std::size_t end_t, std::size_t increment_t, typename function_t>
    constexpr void constexpr_for(function_t&& function)
    {
        if constexpr (start_t < end_t) {
            function(std::integral_constant<decltype(start_t), start_t>());
            constexpr_for<start_t + increment_t, end_t, increment_t>(function);
        }
    }

    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, values_t&... values)
    {
        std::tuple<values_t&...> _tuple((values)...);
        constexpr std::size_t _count = std::variant_size_v<std::variant<values_t...>>;
        constexpr_for<0, _count, 1>([&](const std::size_t _index) {
            using child_type_t = std::variant_alternative_t<_index, std::variant<values_t...>>;
            child_type_t& _child = std::get<_index>(_tuple);
            function(_child);
        });
    }
}
}