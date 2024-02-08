#pragma once

#include <variant>

#include <bungeegum/glue/toolchain.hpp>

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

#if !TOOLCHAIN_COMPILER_CLANG // todo fix !
    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, values_t&... values)
    {
        std::tuple<values_t&...> _tuple((values)...);
        constexpr std::size_t _count = std::variant_size_v<std::variant<values_t...>>;
        constexpr_for<0, _count, 1>([&](constexpr std::size_t _index) {
            std::integral_constant<std::size_t, _index> _constexpr_index;
            using child_type_t = std::variant_alternative_t<_constexpr_index, std::variant<values_t...>>;
            child_type_t& _child = std::get<_constexpr_index()>(_tuple);
            function(_child);
        });
    }
#endif

}
}