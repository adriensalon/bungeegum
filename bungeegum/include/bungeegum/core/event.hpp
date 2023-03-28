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

    event();
    event(const on_trigger_callback& trigger_callback);
    event(const event& other);
    event& operator=(const event& other);
    event(event&& other);
    event& operator=(event&& other);
    ~event();

    /// @brief
    /// @details
    event& merge(const event& other);

    /// @brief
    /// @details
    event& operator+=(const event& other);

    /// @brief Transfers ownership of the underlying callback dispatcher back to this event object
    /// @details This allows you to
    event& attach();

    /// @brief Transfers ownership of the underlying callback dispatcher to the specified widget
    /// @details This allows you to let this event object exit scope without destroying its
    /// callback dispatch subroutine. While an event is detached to a widget both will be destroyed
    /// at the same time
    /// @tparam widget_t
    /// @param widget
    template <typename widget_t>
    event& detach(widget_t& widget);

    /// @brief Transfers ownership of the underlying callback dispatcher to the internal registry
    /// @details This allows you to let this event object exit scope without destroying its
    /// callback dispatch subroutine. While an event is detached to the internal registry it will
    /// be destroyed when the application terminates
    event& detach();

    /// @brief Emplaces a new callback to be fired after
    /// @param trigger_callback
    event& on_trigger(const on_trigger_callback& trigger_callback);

    /// @brief Triggers all contained callbacks
    /// @param ...values
    const event& trigger(values_t&&... values) const; // + operator() overload

    /// @brief
    /// @param future_value
    void trigger(std::future<future_values>&& future_value); // + operator() overload

    /// @brief
    /// @param shared_future_value
    void trigger(const std::shared_future<future_values>& shared_future_value); // + operator() overload

    /// @brief
    std::vector<on_trigger_callback>& trigger_callbacks();

    /// @brief
    const std::vector<on_trigger_callback>& trigger_callbacks() const;

private:
    detail::typed_event_data<values_t...>& _data;
};

// /// @brief
// /// @tparam ...values_t
// /// @tparam widget_t
// /// @param widget
// /// @return
// template <typename widget_t, typename... values_t>
// [[nodiscard]] event<values_t...>& make_event(widget_t* widget);

// /// @brief
// /// @tparam widget_t
// /// @tparam ...values_t
// /// @param widget
// /// @param trigger_callback
// /// @return
// template <typename widget_t, typename... values_t>
// [[nodiscard]] event<values_t...>& make_event(widget_t* widget, typename const event<values_t...>::on_trigger_callback& trigger_callback);

// /// @brief
// /// @tparam widget_t
// /// @tparam ...values_t
// /// @param widget
// /// @param other
// /// @return
// template <typename widget_t, typename... values_t>
// [[nodiscard]] event<values_t...>& make_event(widget_t* widget, const event<values_t...>& other);

// + move
}

#include <bungeegum/core/event.inl>
