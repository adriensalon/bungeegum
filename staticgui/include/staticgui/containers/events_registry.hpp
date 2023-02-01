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
#include <optional>
#include <typeindex>
#include <vector>

#include <staticgui/glue/registry.hpp>

namespace staticgui {
namespace detail {

    struct event_disabled {
    };

    template <typename... values_t>
    struct event_impl {
        std::vector<std::function<void(values_t...)>> callbacks;
        // std::vector<std::future<std::tuple<values_t...>>> futures;
    };

    struct event_data {
        std::function<void()> tick = nullptr;
        std::vector<std::type_index> kinds;
        std::optional<std::reference_wrapper<event_data>> parent = std::nullopt;
        std::vector<std::reference_wrapper<event_data>> children = {};
    };

    struct events_registry {
        events_registry();
        events_registry(const events_registry& other) = delete;
        events_registry& operator=(const events_registry& other) = delete;
        events_registry(events_registry&& other);
        events_registry& operator=(events_registry&& other);

        void tick_all();

        template <typename... values_t>
        event_impl<values_t...>& make_event_and_data();

        template <typename... values_t>
        event_data& get_data(event_impl<values_t...>& widget);

        template <typename... values_t>
        event_impl<values_t...>& get_event(event_data& data);

        unsigned int get_depth(event_data& data);

        void iterate_datas(const std::function<void(event_data&)>& iterate_callback);

        template <typename... values_t>
        void iterate_events(const std::function<void(event_impl<values_t...>&)>& iterate_callback);

    private:
        glue::registry _registry;
        std::vector<std::reference_wrapper<event_data>> _roots;
    };

}
}

#include <staticgui/containers/events_registry.inl>
