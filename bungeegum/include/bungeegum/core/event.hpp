#pragma once

#include <bungeegum/core/event.fwd>

namespace bungeegum {
/// @brief Event objects store same type callbacks and can be passed around the user code to be
/// triggered all at once when desired. Events can be fired on std::future completion, taking its
/// return value as input parameters for callbacks
/// @details Lifetime
/// @tparam ...values_t
template <typename... values_t>
struct event {

    /// @brief
    using on_trigger_callback = std::function<void(const values_t&...)>;

    /// @brief
    using future_values = detail::future_typelist_t<values_t...>;

    /// @brief
    /// @details
    event& merge(const event& other);

    /// @brief Emplaces a new callback to be fired after
    /// @param trigger_callback
    event& on_trigger(const on_trigger_callback& trigger_callback);

    /// @brief Triggers all contained callbacks
    /// @param ...values
    event& trigger(values_t&&... values) const;

    /// @brief
    /// @param future_value
    event& trigger(std::future<future_values>&& future_value);

    /// @brief
    /// @param shared_future_value
    event& trigger(const std::shared_future<future_values>& shared_future_value);

    /// @brief
    event& clear();

    /// @brief
    std::vector<on_trigger_callback>& callbacks();

    /// @brief
    const std::vector<on_trigger_callback>& callbacks() const;

private:
    detail::typed_event_data<values_t...> _data;
};

/// @brief
/// @tparam ...values_t
template <typename... values_t>
[[nodiscard]] event<values_t...>& make_event();

template <typename... values_t>
[[nodiscard]] event<values_t...>& make_event(const event<values_t...>& other);

template <typename... values_t>
[[nodiscard]] event<values_t...>& make_event(event<values_t...>&& other);

}

#include <bungeegum/core/event.inl>
