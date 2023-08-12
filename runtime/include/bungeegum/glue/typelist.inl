#pragma once

namespace bungeegum {

namespace detail {
    template <typename head_type_t, typename... tail_types_t>
    struct front_helper<typelist<head_type_t, tail_types_t...>> {
        using type_t = head_type_t;
    };

    template <typename head_type_t, typename... tail_types_t>
    struct pop_front_helper<typelist<head_type_t, tail_types_t...>> {
        using type_t = typelist<tail_types_t...>;
    };

    template <typename... existing_types_t, typename new_type_t>
    struct push_front_helper<typelist<existing_types_t...>, new_type_t> {
        using type_t = typelist<new_type_t, existing_types_t...>;
    };

    template <typename... recursive_types_t>
    struct at_helper<0, typelist<recursive_types_t...>> {
        using type_t = typename front_helper<typelist<recursive_types_t...>>::type_t;
    };

    template <std::size_t index_t, typename... recursive_types_t>
    struct at_helper<index_t, typelist<recursive_types_t...>> {
        using type_t = typename at_helper<index_t - 1, typename pop_front_helper<typelist<recursive_types_t...>>::type_t>::type_t;
    };

    template <typename... types_t>
    struct back_helper<typelist<types_t...>> {
        using type_t = typename at_helper<sizeof...(types_t) - 1, typelist<types_t...>>::type_t;
    };

    template <typename... head_types_t, typename tail_type_t>
    struct pop_back_helper<typelist<head_types_t..., tail_type_t>> {
        using type_t = typelist<head_types_t...>;
    };

    template <typename... existing_types_t, typename new_type_t>
    struct push_back_helper<typelist<existing_types_t...>, new_type_t> {
        using type_t = typelist<existing_types_t..., new_type_t>;
    };

    template <>
    struct typelist<> {
        template <template <typename...> typename typelist_t>
        using as_type = typelist_t<>;

        template <template <typename> typename wrapper_type_t>
        using wrap_type = typelist<>;

        using front_type = void;

        using pop_front_type = typelist<>;

        template <typename new_type_t>
        using push_front_type = typelist<new_type_t>;

        template <std::size_t index_t>
        using at_type = void;

        using back_type = void;

        using pop_back_type = typelist<>;

        template <typename new_type_t>
        using push_back_type = typelist<new_type_t>;

        static constexpr std::size_t size() { return 0; }

        static constexpr bool empty() { return true; }
    };

    template <typename... types_t>
    template <template <typename...> typename typelist_t>
    constexpr typename typelist<types_t...>::template as_type<typelist_t> typelist<types_t...>::as()
    {
        return as_type<typelist_t>();
    }

    template <typename... types_t>
    template <template <typename> typename wrapper_type_t>
    constexpr typename typelist<types_t...>::template wrap_type<wrapper_type_t> typelist<types_t...>::wrap()
    {
        return wrap_type<wrapper_type_t>();
    }

    template <typename... types_t>
    constexpr typename typelist<types_t...>::front_type typelist<types_t...>::front()
    {
        static_assert(!empty(), "BAD USAGE (calling front() on empty typelist)");
        return front_type();
    }

    template <typename... types_t>
    constexpr typename typelist<types_t...>::pop_front_type typelist<types_t...>::pop_front()
    {
        static_assert(!empty(), "BAD USAGE (calling pop_front() on empty typelist)");
        return pop_front_type();
    }

    template <typename... types_t>
    template <typename new_type_t>
    constexpr typename typelist<types_t...>::template push_front_type<new_type_t> typelist<types_t...>::push_front()
    {
        return push_front_type<new_type_t>();
    }

    template <typename... types_t>
    template <std::size_t index_t>
    constexpr typename typelist<types_t...>::template at_type<index_t> typelist<types_t...>::at()
    {
        static_assert(!empty(), "BAD USAGE (calling at() on empty typelist)");
        return at_type<index_t>();
    }

    template <typename... types_t>
    constexpr typename typelist<types_t...>::back_type typelist<types_t...>::back()
    {
        static_assert(!empty(), "BAD USAGE (calling back() on empty typelist)");
        return back_type();
    }

    // template <typename... types_t>
    // constexpr typename typelist<types_t...>::pop_back_type typelist<types_t...>::pop_back()
    // {
    //     static_assert(!empty(), "BAD USAGE (calling pop_back() on empty typelist)");
    //     return pop_back_type();
    // }

    template <typename... types_t>
    template <typename new_type_t>
    constexpr typename typelist<types_t...>::template push_back_type<new_type_t> typelist<types_t...>::push_back()
    {
        return push_back_type<new_type_t>();
    }

    template <typename... types_t>
    constexpr std::size_t typelist<types_t...>::size()
    {
        return (sizeof...(types_t));
    }

    template <typename... types_t>
    constexpr bool typelist<types_t...>::empty()
    {
        return (size() == 0);
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    template <template <typename...> typename new_typelist_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::as()
    {
        return typename typelist<types_t...>::template as_type<new_typelist_t>();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    template <template <typename> typename wrapper_type_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::wrap()
    {
        return typename typelist<types_t...>::template wrap_type<wrapper_type_t>::template as_type<typelist_t>();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::front()
    {
        static_assert(!empty(), "BAD USAGE (calling front() on empty typelist)");
        return typename typelist<types_t...>::front_type();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::pop_front()
    {
        static_assert(!empty(), "BAD USAGE (calling pop_front() on empty typelist)");
        return typename typelist<types_t...>::pop_front_type::template as_type<typelist_t>();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    template <typename new_type_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::push_front()
    {
        return typename typelist<types_t...>::template push_front_type<new_type_t>::template as_type<typelist_t>();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    template <std::size_t index_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::at()
    {
        static_assert(!empty(), "BAD USAGE (calling at() on empty typelist)");
        return typename typelist<types_t...>::template at_type<index_t>();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::back()
    {
        static_assert(!empty(), "BAD USAGE (calling back() on empty typelist)");
        return typename typelist<types_t...>::back_type();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::pop_back()
    {
        static_assert(!empty(), "BAD USAGE (calling pop_back() on empty typelist)");
        return typename typelist<types_t...>::pop_back_type::template as_type<typelist_t>();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    template <typename new_type_t>
    constexpr auto strong_typelist<typelist_t, types_t...>::push_back()
    {
        return typename typelist<types_t...>::template push_back_type<new_type_t>::template as_type<typelist_t>();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    constexpr std::size_t strong_typelist<typelist_t, types_t...>::size()
    {
        return typelist<types_t...>::size();
    }

    template <template <typename...> typename typelist_t, typename... types_t>
    constexpr bool strong_typelist<typelist_t, types_t...>::empty()
    {
        return typelist<types_t...>::empty();
    }
}
}