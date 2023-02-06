//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <chrono>

namespace staticgui {
namespace detail {

    template <typename... values_t>
    event_impl<values_t...>& event_registry::make_event_and_data()
    {
        glue::entity _entity = _registry.create_entity();
        event_impl<values_t...>& _event = _registry.create_component<event_impl<values_t...>>(_entity);
        event_data& _event_data = _registry.create_component<event_data>(_entity);
        _event_data.tick = [&]() {
            // std::cout << "tick... !! \n";
            for (auto _future_it = _event.futures.begin(); _future_it != _event.futures.end();) {
                if (_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    std::cout << "READY !! \n";
                    if constexpr (typename value_or_tuple<values_t...>::is_void()) {
                        for (auto& _callback : _event.callbacks)
                            _callback();
                    } else {
                        typename value_or_tuple<values_t...>::type _vals = _future_it->get();
                        if constexpr (typename value_or_tuple<values_t...>::is_tuple()) {
                            for (auto& _callback : _event.callbacks)
                                _callback(std::forward<values_t>(std::get<values_t>(_vals))...);
                        } else {
                            for (auto& _callback : _event.callbacks)
                                _callback(std::forward<values_t>(_vals)...);
                        }
                    }
                    _future_it = _event.futures.erase(_future_it);
                } else
                    _future_it++;
            }
            for (auto& _shared_future : _event.shared_futures) {
                if (_shared_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    // return true ?
                }
            }
        };
        (_event_data.kinds.emplace_back(typeid(values_t)), ...);
        return _event;
    }

    template <typename... values_t>
    void event_registry::destroy_event_and_data(const event_impl<values_t...>& event)
    {
        glue::entity _entity = _registry.get_entity(event);
        // _registry. DESTROY
    }

    template <typename... values_t>
    void event_registry::trigger_values(event_impl<values_t...>& event, values_t&&... values)
    {
        for (auto& _callback : event.callbacks)
            _callback(std::forward<values_t>(values)...);
    }

    template <typename... values_t>
    void event_registry::trigger_future_value(event_impl<values_t...>& event, std::future<typename value_or_tuple<values_t...>::type>&& future_value)
    {
        event.futures.push_back(std::move(future_value));
    }

    template <typename... values_t>
    void event_registry::trigger_shared_future_value(event_impl<values_t...>& event, const std::shared_future<typename value_or_tuple<values_t...>::type>& shared_future_value)
    {
        event.shared_futures.push_back(shared_future_value);
    }
}
}