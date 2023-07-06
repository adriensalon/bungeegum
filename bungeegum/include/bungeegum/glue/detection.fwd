#pragma once

// THE C++ DETECTION IDIOM

// https://en.cppreference.com/w/cpp/experimental/is_detected

// These will no longer be useful with C++20 thanks to concepts (!)

#include <type_traits>

namespace bungeegum {
namespace detail {

    template <typename default_t, typename always_void_t, template <typename...> typename op_t, typename... args_t>
    struct detector {
        using value_t = std::false_type;
        using type = default_t;
    };

    template <typename default_t, template <typename...> typename op_t, typename... args_t>
    struct detector<default_t, std::void_t<op_t<args_t...>>, op_t, args_t...> {
        using value_t = std::true_type;
        using type = op_t<args_t...>;
    };

    struct nonesuch {
        nonesuch() = delete;
        ~nonesuch() = delete;
        nonesuch(nonesuch const&) = delete;
        void operator=(nonesuch const&) = delete;
    };

    template <template <typename...> typename op_t, typename... args_t>
    using is_detected = typename detail::detector<detail::nonesuch, void, op_t, args_t...>::value_t;

    template <template <typename...> typename op_t, typename... args_t>
    using detected_t = typename detail::detector<detail::nonesuch, void, op_t, args_t...>::type;

    template <typename default_t, template <typename...> typename op_t, typename... args_t>
    using detected_or = detail::detector<default_t, void, op_t, args_t...>;

    // additional utilities

    template <template <typename...> typename op_t, typename... args_t>
    constexpr inline bool is_detected_v = is_detected<op_t, args_t...>::value;

    template <typename default_t, template <typename...> typename op_t, typename... args_t>
    using detected_or_t = typename detected_or<default_t, op_t, args_t...>::type;

    template <typename expected_t, template <typename...> typename op_t, typename... args_t>
    using is_detected_exact = std::is_same<expected_t, detected_t<op_t, args_t...>>;

    template <typename expected_t, template <typename...> typename op_t, typename... args_t>
    constexpr inline bool is_detected_exact_v = is_detected_exact<expected_t, op_t, args_t...>::value;

    template <typename to_t, template <typename...> typename op_t, typename... args_t>
    using is_detected_convertible = std::is_convertible<detected_t<op_t, args_t...>, to_t>;

    template <typename to_t, template <typename...> typename op_t, typename... args_t>
    constexpr inline bool is_detected_convertible_v = is_detected_convertible<to_t, op_t, args_t...>::value;
}
}
