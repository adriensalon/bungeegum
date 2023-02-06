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
#include <vector>

#include <staticgui/glue/registry.hpp>
#include <staticgui/state/traits.hpp>

namespace staticgui {
namespace detail {

    struct event_disabled {
    };

    template <typename... values_t>
    struct event_impl {
        using future_type = typename value_or_tuple<values_t...>::type;

        event_impl() { }
        event_impl(const event_impl& other) = delete;
        event_impl& operator=(const event_impl& other) = delete;
        event_impl(event_impl&& other)
        {
            *this = std::move(other);
        }
        event_impl& operator=(event_impl&& other)
        {
            is_attached = other.is_attached;
            callbacks = std::move(other.callbacks);
            futures = std::move(other.futures);
            shared_futures = std::move(other.shared_futures);
            return *this;
        }

        bool is_attached = true;
        std::vector<std::function<void(values_t...)>> callbacks;
        std::vector<std::future<future_type>> futures;
        std::vector<std::shared_future<future_type>> shared_futures;
    };

    struct event_data {
        std::function<void()> tick = nullptr;
        std::vector<std::type_index> kinds;
        std::optional<std::reference_wrapper<event_data>> parent = std::nullopt;
        std::vector<std::reference_wrapper<event_data>> children = {};
    };

    struct event_registry {
        event_registry();
        event_registry(const event_registry& other) = delete;
        event_registry& operator=(const event_registry& other) = delete;
        event_registry(event_registry&& other);
        event_registry& operator=(event_registry&& other);

        void tick();

        template <typename... values_t>
        event_impl<values_t...>& make_event_and_data();

        template <typename... values_t>
        void destroy_event_and_data(const event_impl<values_t...>& event);

        template <typename... values_t>
        event_data& get_data(event_impl<values_t...>& widget);

        template <typename... values_t>
        event_impl<values_t...>& get_event(event_data& data);

        void iterate_datas(const std::function<void(event_data&)>& iterate_callback);

        template <typename... values_t>
        void iterate_events(const std::function<void(event_impl<values_t...>&)>& iterate_callback);

        unsigned int get_depth(event_data& data);

        template <typename... values_t>
        void trigger_values(event_impl<values_t...>& event, values_t&&... values);

        template <typename... values_t>
        void trigger_future_value(event_impl<values_t...>& event, std::future<typename value_or_tuple<values_t...>::type>&& future_value);

        template <typename... values_t>
        void trigger_shared_future_value(event_impl<values_t...>& event, const std::shared_future<typename value_or_tuple<values_t...>::type>& shared_future_value);

    private:
        glue::registry _registry;
    };

}
}

#include <staticgui/state/event.inl>
