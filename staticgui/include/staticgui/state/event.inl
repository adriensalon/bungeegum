//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <algorithm>
#include <chrono>

namespace staticgui {
namespace detail {

    template <typename... values_t>
    event_impl<values_t...>::event_impl() { }

    template <typename... values_t>
    event_impl<values_t...>::event_impl(event_impl&& other)
    {
        *this = std::move(other);
    }

    template <typename... values_t>
    event_impl<values_t...>& event_impl<values_t...>::operator=(event_impl<values_t...>&& other)
    {
        is_attached = other.is_attached;
        callbacks = std::move(other.callbacks);
        futures = std::move(other.futures);
        shared_futures = std::move(other.shared_futures);
        return *this;
    }

    template <typename... values_t>
    event_impl<values_t...>& event_registry::make_event_and_data()
    {
        glue::entity _entity = _registry.create_entity();
        event_impl<values_t...>& _event = _registry.create_component<event_impl<values_t...>>(_entity);
        event_data& _event_data = _registry.create_component<event_data>(_entity);
        _event_data.tick = [&]() {
            for (auto _future_it = _event.futures.begin(); _future_it != _event.futures.end();) {
                if (_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    if constexpr (glue::future_typelist<values_t...>::is_void()) {
                        for (auto& _callback : _event.callbacks)
                            _callback();
                    } else {
                        glue::future_typelist_t<values_t...> _vals = _future_it->get();
                        if constexpr (glue::future_typelist<values_t...>::is_tuple()) {
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
            for (auto _shared_future_it = _event.shared_futures.begin(); _shared_future_it != _event.shared_futures.end();) {
                if (_shared_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {

                    if constexpr (glue::future_typelist<values_t...>::is_void()) {
                        for (auto& _callback : _event.callbacks)
                            _callback();
                    } else {
                        glue::future_typelist_t<values_t...> _vals = _shared_future_it->get();
                        if constexpr (glue::future_typelist<values_t...>::is_tuple()) {
                            for (auto& _callback : _event.callbacks)
                                _callback(std::forward<values_t>(std::get<values_t>(_vals))...);
                        } else {
                            for (auto& _callback : _event.callbacks)
                                _callback(std::forward<values_t>(_vals)...);
                        }
                    }
                    _shared_future_it = _event.shared_futures.erase(_shared_future_it);
                } else
                    _shared_future_it++;
            }
        };
        (_event_data.kinds.emplace_back(typeid(values_t)), ...);
        return _event;
    }

    template <typename... values_t>
    void event_registry::merge_events_and_datas(event_impl<values_t...>& merger, event_impl<values_t...>& merged)
    {
        // attach_to_wrapper(merger);
        // attach_to_wrapper(merged);
        merger.callbacks.insert(merger.callbacks.end(),
            std::make_move_iterator(merged.callbacks.begin()),
            std::make_move_iterator(merged.callbacks.end()));
        merger.futures.insert(merger.futures.end(),
            std::make_move_iterator(merged.futures.begin()),
            std::make_move_iterator(merged.futures.end()));
        merger.shared_futures.insert(merger.shared_futures.end(),
            std::make_move_iterator(merged.shared_futures.begin()),
            std::make_move_iterator(merged.shared_futures.end()));
        // destroy_event_and_data(merged);
    }

    template <typename... values_t>
    void event_registry::destroy_event_and_data(const event_impl<values_t...>& event)
    {
        glue::entity _entity = _registry.get_entity(event);
        _registry.destroy_entity_components(_entity);
    }

    template <typename... values_t>
    event_data& event_registry::get_data(event_impl<values_t...>& event)
    {
        glue::entity _entity = _registry.get_entity(event);
        return _registry.get_component<event_data>(_entity);
    }

    template <typename... values_t>
    void event_registry::trigger_values(event_impl<values_t...>& event, values_t&&... values)
    {
        for (auto& _callback : event.callbacks)
            _callback(std::forward<values_t>(values)...);
    }

    template <typename... values_t>
    void event_registry::trigger_future_value(event_impl<values_t...>& event, std::future<future_values<values_t...>>&& future_value)
    {
        event.futures.push_back(std::move(future_value));
    }

    template <typename... values_t>
    void event_registry::trigger_shared_future_value(event_impl<values_t...>& event, const std::shared_future<future_values<values_t...>>& shared_future_value)
    {
        event.shared_futures.push_back(shared_future_value);
    }

    template <typename... values_t>
    void event_registry::attach_to_wrapper(event_impl<values_t...>& event)
    {
        if (!event.is_attached)
            event.rattach_callback();
        event.is_attached = true;
        event.rattach_callback = []() {};
    }

    template <typename... values_t>
    void event_registry::detach_to_registry(event_impl<values_t...>& event)
    {
        if (!event.is_attached)
            event.rattach_callback();
        event.is_attached = false;
        event.detached_id = glue::generator::create();
        event_data& _data = get_data(event);
        _detached_events.emplace(event.detached_id, _data);
        event.rattach_callback = [&]() {
            _detached_events.erase(event.detached_id);
            glue::generator::destroy(event.detached_id);
            event.detached_id = 0;
        };
    }
}
}