//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

// https://en.cppreference.com/w/cpp/experimental/is_detected

#include <type_traits>

namespace staticgui {
namespace detail {
    namespace detail {

        template <typename Default, typename AlwaysVoid, template <typename...> typename Op, typename... Args>
        struct detector {
            using value_t = std::false_type;
            using type = Default;
        };

        template <typename Default, template <typename...> typename Op, typename... Args>
        struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
            using value_t = std::true_type;
            using type = Op<Args...>;
        };

        struct nonesuch {
            nonesuch() = delete;
            ~nonesuch() = delete;
            nonesuch(nonesuch const&) = delete;
            void operator=(nonesuch const&) = delete;
        };

    }

    template <template <typename...> typename Op, typename... Args>
    using is_detected = typename detail::detector<detail::nonesuch, void, Op, Args...>::value_t;

    template <template <typename...> typename Op, typename... Args>
    using detected_t = typename detail::detector<detail::nonesuch, void, Op, Args...>::type;

    template <typename Default, template <typename...> typename Op, typename... Args>
    using detected_or = detail::detector<Default, void, Op, Args...>;

    // additional utilities

    template <template <typename...> typename Op, typename... Args>
    constexpr inline bool is_detected_v = is_detected<Op, Args...>::value;

    template <typename Default, template <typename...> typename Op, typename... Args>
    using detected_or_t = typename detected_or<Default, Op, Args...>::type;

    template <typename Expected, template <typename...> typename Op, typename... Args>
    using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

    template <typename Expected, template <typename...> typename Op, typename... Args>
    constexpr inline bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

    template <typename To, template <typename...> typename Op, typename... Args>
    using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

    template <typename To, template <typename...> typename Op, typename... Args>
    constexpr inline bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;

}
}
