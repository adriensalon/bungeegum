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

    /// @brief
    /// @details
    event& merge(event&& other);

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

    /// @brief This is required because of how entt works : a chain of constructor, move
    /// constructor, destructor of an object that is in an unspecified but safe state, see
    /// https://github.com/skypjack/entt/issues/488. Idk precisely how but this seems to imply
    /// that this pointer has not the same void* value in destructor and regular member functions
    /// @details An alternative would be entt meta destructors (invoked before dtors), see
    /// https://skypjack.github.io/entt/md_docs_md_meta.html
    virtual ~event();

private:
    detail::typed_event_data<values_t...> _data;
    friend struct detail::events_registry;
};

/// @brief
/// @tparam ...values_t
template <typename... values_t>
[[nodiscard]] event<values_t...>& make_event();

/// @brief
/// @tparam ...values_t
/// @param other_event
template <typename... values_t>
[[nodiscard]] event<values_t...>& make_event(const event<values_t...>& other_event);

/// @brief
/// @tparam ...values_t
/// @param other_event
template <typename... values_t>
[[nodiscard]] event<values_t...>& make_event(event<values_t...>&& other_event);

/// @brief
/// @tparam ...values_t
/// @param event
template <typename... values_t>
void unmake_event(event<values_t...>& event);
}

#include <bungeegum/core/event.inl>
