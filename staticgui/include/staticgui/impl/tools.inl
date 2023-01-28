//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <variant>

namespace staticgui {
namespace tools {

    template <unsigned int start_t, unsigned int end_t, unsigned int increment_t, typename function_t>
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
        std::tuple<values_t&...> _tuple(std::forward<values_t&>(values)...);
        constexpr size_t _count = std::variant_size_v<std::variant<values_t...>>;
        constexpr_for<0, _count, 1>([&](auto _index) {
            // using child_type_t = std::variant_alternative_t<_index, std::variant<values_t...>>;
            auto& _child = (std::get<_index>(_tuple));
            function(_child);
        });
    }

    template <typename... values_t, typename function_t>
    constexpr void constexpr_foreach(function_t&& function, const values_t&... values)
    {
        std::tuple<const values_t&...> _tuple(std::forward<const values_t&>(values)...);
        constexpr size_t _count = std::variant_size_v<std::variant<values_t...>>;
        constexpr_for<0, _count, 1>([&](auto _index) {
            // using child_type_t = std::variant_alternative_t<_index, std::variant<values_t...>>;
            auto& _child = (std::get<_index>(_tuple));
            function(_child);
        });
    }

}
}