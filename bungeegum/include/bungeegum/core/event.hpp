#pragma once

#include <bungeegum/core/event.fwd>

namespace bungeegum {

/// @brief Event objects store same type callbacks and can be passed around the user code to be
/// triggered all at once when desired. Events can be fired on std::future completion, taking its
/// return value as input parameters for callbacks.
/// @details Event object instances can be copied (deep copy of the callbacks, but the futures and
/// shared futures are not copied) and moved.
/// @tparam ...values_t are the types to use as arguments for callbacks.
template <typename... values_t>
struct event {

    /// @brief Instances of this type are the callback objects.
    using on_trigger_callback = std::function<void(const values_t&...)>;

    /// @brief Future values resolve to void for event<>, to value_t for event<value_t> and to
    /// std::tuple<values_t...> for event<values_t...> so that it fits as a template parameter
    /// for std::future and std::shared_future
    using future_values = detail::future_typelist_t<values_t...>;

    /// @brief Merges another event by adding its callbacks.
    event& merge(const event& other);

    /// @brief Merges another event by adding its callbacks.
    event& merge(event&& other);

    /// @brief Emplaces a new callback to be fired when trigger() is called.
    event& on_trigger(const on_trigger_callback& trigger_callback);

    /// @brief Triggers all contained callbacks with the provided values.
    event& trigger(values_t&&... values) const;

    /// @brief Triggers all contained callbacks on future completion with the resulting values.
    event& trigger(std::future<future_values>&& future_value);

    /// @brief Triggers all contained callbacks on future completion with the resulting values.
    event& trigger(const std::shared_future<future_values>& shared_future_value);

    /// @brief Gets a reference to stored callbacks.
    std::vector<on_trigger_callback>& callbacks();

    /// @brief Gets a const reference to stored callbacks.
    const std::vector<on_trigger_callback>& callbacks() const;

private:
    detail::event_data<values_t...> _data;
};
}

#include <bungeegum/core/event.inl>
