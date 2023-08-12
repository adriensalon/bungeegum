#pragma once

#include <cstddef>
#include <tuple>

namespace bungeegum {

namespace detail {
    template <typename typelist_t>
    struct front_helper;

    template <typename typelist_t>
    struct pop_front_helper;

    template <typename typelist_t, typename new_type_t>
    struct push_front_helper;

    template <std::size_t index_t, typename... recursive_types_t>
    struct at_helper;

    template <typename typelist_t>
    struct back_helper;

    template <typename typelist_t>
    struct pop_back_helper;

    template <typename typelist_t, typename new_type_t>
    struct push_back_helper;

    template <typename... types_t>
    struct typelist {
        template <template <typename...> typename typelist_t>
        using as_type = typelist_t<types_t...>;

        template <template <typename> typename wrapper_type_t>
        using wrap_type = typelist<wrapper_type_t<types_t>...>;

        using front_type = typename detail::front_helper<typelist<types_t...>>::type_t;

        using pop_front_type = typename detail::pop_front_helper<typelist>::type_t;

        template <typename new_type_t>
        using push_front_type = typename detail::push_front_helper<typelist, new_type_t>::type_t;

        template <std::size_t index_t>
        using at_type = typename detail::at_helper<index_t, typelist>::type_t;

        using back_type = typename detail::back_helper<typelist>::type_t;

        // using pop_back_type = typename detail::pop_back_helper<typelist>::type_t;

        template <typename new_type_t>
        using push_back_type = typename detail::push_back_helper<typelist, new_type_t>::type_t;

        template <template <typename...> typename typelist_t>
        [[nodiscard]] static constexpr as_type<typelist_t> as();

        template <template <typename> typename wrapper_type_t>
        [[nodiscard]] static constexpr wrap_type<wrapper_type_t> wrap();

        [[nodiscard]] static constexpr front_type front();

        [[nodiscard]] static constexpr pop_front_type pop_front();

        template <typename new_type_t>
        [[nodiscard]] static constexpr push_front_type<new_type_t> push_front();

        template <std::size_t index_t>
        [[nodiscard]] static constexpr at_type<index_t> at();

        [[nodiscard]] static constexpr back_type back();

        // [[nodiscard]] static constexpr pop_back_type pop_back();

        template <typename new_type_t>
        [[nodiscard]] static constexpr push_back_type<new_type_t> push_back();

        [[nodiscard]] static constexpr std::size_t size();

        [[nodiscard]] static constexpr bool empty();
    };

    template <template <typename...> typename typelist_t, typename... types_t>
    struct strong_typelist {
        template <template <typename...> typename new_typelist_t>
        [[nodiscard]] static constexpr auto as();

        template <template <typename> typename wrapper_type_t>
        [[nodiscard]] static constexpr auto wrap();

        [[nodiscard]] static constexpr auto front();

        [[nodiscard]] static constexpr auto pop_front();

        template <typename new_type_t>
        [[nodiscard]] static constexpr auto push_front();

        template <std::size_t index_t>
        [[nodiscard]] static constexpr auto at();

        [[nodiscard]] static constexpr auto back();

        [[nodiscard]] static constexpr auto pop_back();

        template <typename new_type_t>
        [[nodiscard]] static constexpr auto push_back();

        [[nodiscard]] static constexpr std::size_t size();

        [[nodiscard]] static constexpr bool empty();
    };

    template <typename... types_t>
    struct future_typelist {
        constexpr static bool is_void() { return false; }
        constexpr static bool is_value() { return false; }
        constexpr static bool is_tuple() { return true; }
        using type = std::tuple<types_t...>;
    };

    template <typename type_t>
    struct future_typelist<type_t> {
        constexpr static bool is_void() { return false; }
        constexpr static bool is_value() { return true; }
        constexpr static bool is_tuple() { return false; }
        using type = type_t;
    };

    template <>
    struct future_typelist<> {
        constexpr static bool is_void() { return true; }
        constexpr static bool is_value() { return false; }
        constexpr static bool is_tuple() { return false; }
        using type = void;
    };

    template <typename... types_t>
    using future_typelist_t = typename future_typelist<types_t...>::type;
}
}

#include <bungeegum/glue/typelist.inl>
