//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>
#include <future>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <bungeegum/glue/registry.hpp>
#include <bungeegum/glue/typelist.hpp>

namespace bungeegum {
namespace detail {

    template <typename... values_t>
    struct typed_event_data {
        typed_event_data();
        typed_event_data(const typed_event_data& other) = delete;
        typed_event_data& operator=(const typed_event_data& other) = delete;
        typed_event_data(typed_event_data&& other);
        typed_event_data& operator=(typed_event_data&& other);

        bool is_attached = true;
        entity_t detached_id = 0;
        std::function<void()> rattach_callback = nullptr;
        std::vector<std::function<void(values_t...)>> callbacks;
        std::vector<std::future<future_typelist_t<values_t...>>> futures;
        std::vector<std::shared_future<future_typelist_t<values_t...>>> shared_futures;
    };

    template <typename... values_t>
    void merge(typed_event_data<values_t...>& merger, typed_event_data<values_t...>& merged);

    template <typename... values_t>
    void trigger(typed_event_data<values_t...>& event, values_t&&... values);

    template <typename... values_t>
    void trigger(typed_event_data<values_t...>& event, std::future<future_typelist_t<values_t...>>&& future_value);

    template <typename... values_t>
    void trigger(typed_event_data<values_t...>& event, const std::shared_future<future_typelist_t<values_t...>>& shared_future_value);

    template <typename... values_t>
    void attach(typed_event_data<values_t...>& event);

    struct untyped_event_data {
        std::vector<std::type_index> kinds;
        std::function<void()> tick = nullptr;
    };

    struct events_registry {
        registry events;
        std::unordered_map<entity_t, std::reference_wrapper<untyped_event_data>> detached_events;
        std::unordered_map<entity_t, std::reference_wrapper<untyped_event_data>> detached_events_to_widgets;

        void tick();
        void iterate(const std::function<void(untyped_event_data&)>& iterate_callback);

        template <typename... values_t>
        typed_event_data<values_t...>& create();

        template <typename... values_t>
        void destroy(const typed_event_data<values_t...>& event);

        template <typename... values_t>
        untyped_event_data& get(typed_event_data<values_t...>& event);

        template <typename... values_t>
        typed_event_data<values_t...>& get(untyped_event_data& data);

        template <typename... values_t>
        void iterate(const std::function<void(typed_event_data<values_t...>&)>& iterate_callback);

        template <typename... values_t>
        void detach(typed_event_data<values_t...>& event);
    };

    inline events_registry events_context;

}

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
    using future_values = future_typelist_t<values_t...>;

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
    /// @param future_value
    void trigger(const std::shared_future<future_values>& shared_future_value); // + operator() overload

    /// @brief
    std::vector<on_trigger_callback>& trigger_callbacks();

    /// @brief
    const std::vector<on_trigger_callback>& trigger_callbacks() const;

private:
    detail::typed_event_data<values_t...>& _data;
};

}

#include <bungeegum/context/event.inl>
