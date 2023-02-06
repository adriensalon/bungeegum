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

#include <staticgui/glue/registry.hpp>
#include <staticgui/state/sfinae.hpp>

namespace staticgui {
namespace detail {

    template <typename... values_t>
    struct event_impl {
        using future_type = detail::future_values<values_t...>;

        event_impl();
        event_impl(const event_impl& other) = delete;
        event_impl& operator=(const event_impl& other) = delete;
        event_impl(event_impl&& other);
        event_impl& operator=(event_impl&& other);

        bool is_attached = true;
        glue::entity detached_id = 0;
        std::function<void()> rattach_callback = nullptr;
        std::vector<std::function<void(values_t...)>> callbacks;
        std::vector<std::future<future_type>> futures;
        std::vector<std::shared_future<future_type>> shared_futures;
    };

    struct event_data {
        std::function<void()> tick = nullptr;
        std::vector<std::type_index> kinds;
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
        void merge_events_and_datas(event_impl<values_t...>& merger, event_impl<values_t...>& merged);

        template <typename... values_t>
        void destroy_event_and_data(const event_impl<values_t...>& event);

        template <typename... values_t>
        event_data& get_data(event_impl<values_t...>& event);

        template <typename... values_t>
        event_impl<values_t...>& get_event(event_data& data);

        void iterate_datas(const std::function<void(event_data&)>& iterate_callback);

        template <typename... values_t>
        void iterate_events(const std::function<void(event_impl<values_t...>&)>& iterate_callback);

        template <typename... values_t>
        void trigger_values(event_impl<values_t...>& event, values_t&&... values);

        template <typename... values_t>
        void trigger_future_value(event_impl<values_t...>& event, std::future<future_values<values_t...>>&& future_value);

        template <typename... values_t>
        void trigger_shared_future_value(event_impl<values_t...>& event, const std::shared_future<future_values<values_t...>>& shared_future_value);

        template <typename... values_t>
        void attach_to_wrapper(event_impl<values_t...>& event);

        // template <typename... values_t>
        // void mark_detached_to_widget(event_impl<values_t...>& event, const std::function<void()>& detach_callback);

        template <typename... values_t>
        void detach_to_registry(event_impl<values_t...>& event);

    private:
        glue::registry _registry;
        std::unordered_map<glue::entity, std::reference_wrapper<event_data>> _detached_events;
        std::unordered_map<glue::entity, std::reference_wrapper<event_data>> _detached_events_to_widgets;
    };

}
}

#include <staticgui/state/event.inl>
