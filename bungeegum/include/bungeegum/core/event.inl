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

namespace bungeegum {
namespace detail {

    template <typename... values_t>
    typed_event_data<values_t...>::typed_event_data() { }

    template <typename... values_t>
    typed_event_data<values_t...>::typed_event_data(typed_event_data&& other)
    {
        *this = std::move(other);
    }

    template <typename... values_t>
    typed_event_data<values_t...>& typed_event_data<values_t...>::operator=(typed_event_data<values_t...>&& other)
    {
        is_attached = other.is_attached;
        callbacks = std::move(other.callbacks);
        futures = std::move(other.futures);
        shared_futures = std::move(other.shared_futures);
        return *this;
    }

    template <typename... values_t>
    void merge(typed_event_data<values_t...>& merger, typed_event_data<values_t...>& merged)
    {
        // attach(merger);
        // attach(merged);
        merger.callbacks.insert(merger.callbacks.end(),
            std::make_move_iterator(merged.callbacks.begin()),
            std::make_move_iterator(merged.callbacks.end()));
        merger.futures.insert(merger.futures.end(),
            std::make_move_iterator(merged.futures.begin()),
            std::make_move_iterator(merged.futures.end()));
        merger.shared_futures.insert(merger.shared_futures.end(),
            std::make_move_iterator(merged.shared_futures.begin()),
            std::make_move_iterator(merged.shared_futures.end()));
    }

    template <typename... values_t>
    void trigger(typed_event_data<values_t...>& event, values_t&&... values)
    {
        for (auto& _callback : event.callbacks)
            _callback(std::forward<values_t>(values)...);
    }

    template <typename... values_t>
    void trigger(typed_event_data<values_t...>& event, std::future<future_typelist_t<values_t...>>&& future_value)
    {
        event.futures.push_back(std::move(future_value));
    }

    template <typename... values_t>
    void trigger(typed_event_data<values_t...>& event, const std::shared_future<future_typelist_t<values_t...>>& shared_future_value)
    {
        event.shared_futures.push_back(shared_future_value);
    }

    template <typename... values_t>
    void attach(typed_event_data<values_t...>& event)
    {
        if (!event.is_attached)
            event.rattach_callback();
        event.is_attached = true;
        event.rattach_callback = []() {};
    }

    template <typename... values_t>
    typed_event_data<values_t...>& events_registry::create()
    {
        entity_t _entity = events.create_entity();
        typed_event_data<values_t...>& _event = events.create_component<typed_event_data<values_t...>>(_entity);
        untyped_event_data& _event_data = events.create_component<untyped_event_data>(_entity);
        _event_data.tick = [&]() {
            for (auto _future_it = _event.futures.begin(); _future_it != _event.futures.end();) {
                if (_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    if constexpr (future_typelist<values_t...>::is_void()) {
                        for (auto& _callback : _event.callbacks)
                            _callback();
                    } else {
                        future_typelist_t<values_t...> _vals = _future_it->get();
                        if constexpr (future_typelist<values_t...>::is_tuple()) {
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

                    if constexpr (future_typelist<values_t...>::is_void()) {
                        for (auto& _callback : _event.callbacks)
                            _callback();
                    } else {
                        future_typelist_t<values_t...> _vals = _shared_future_it->get();
                        if constexpr (future_typelist<values_t...>::is_tuple()) {
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
    void events_registry::destroy(const typed_event_data<values_t...>& event)
    {
        entity_t _entity = events.get_entity(event);
        events.destroy_entity(_entity);
    }

    template <typename... values_t>
    untyped_event_data& events_registry::get(typed_event_data<values_t...>& event)
    {
        entity_t _entity = events.get_entity(event);
        return events.get_component<untyped_event_data>(_entity);
    }

    template <typename... values_t>
    void events_registry::detach(typed_event_data<values_t...>& event)
    {
        if (!event.is_attached)
            event.rattach_callback();
        event.is_attached = false;
        event.detached_id = generator::create();
        untyped_event_data& _data = get(event);
        detached_events.emplace(event.detached_id, _data);
        event.rattach_callback = [&]() {
            detached_events.erase(event.detached_id);
            generator::destroy(event.detached_id);
            event.detached_id = 0;
        };
    }
}

template <typename... values_t>
event(std::function<void(values_t...)>) -> event<values_t...>;

template <typename... values_t>
event<values_t...>::event()
    : _data(detail::events_context.create<values_t...>())
{
}

template <typename... values_t>
event<values_t...>::event(const on_trigger_callback& trigger_callback)
    : _data(detail::events_context.create<values_t...>())
{
    _data.callbacks.push_back(trigger_callback);
}

template <typename... values_t>
event<values_t...>::event(const event& other)
{
    *this = other;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::operator=(const event& other)
{
    _data = other._data;
    return *this;
}

template <typename... values_t>
event<values_t...>::event(event&& other)
{
    *this = std::move(other);
}

template <typename... values_t>
event<values_t...>& event<values_t...>::operator=(event<values_t...>&& other)
{
    _data = std::move(other._data);
    return *this;
}

template <typename... values_t>
event<values_t...>::~event()
{
    if (_data.is_attached)
        detail::events_context.destroy(_data);
}

template <typename... values_t>
event<values_t...>& event<values_t...>::merge(const event<values_t...>& other)
{
    detail::merge<values_t...>(_data, other._data);
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::operator+=(const event<values_t...>& other)
{
    return merge(other);
}

template <typename... values_t>
event<values_t...>& event<values_t...>::attach()
{
    detail::attach(_data);
    return *this;
}

// template <typename... values_t>
// template <typename widget_t>
// event<values_t...>& event<values_t...>::detach(widget_t& widget)
// {
//     detail::state.context.widgets.detach_to_widget(_data, widget);
//     return *this;
// }

template <typename... values_t>
event<values_t...>& event<values_t...>::detach()
{
    detail::events_context.detach(_data);
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::on_trigger(const on_trigger_callback& trigger_callback)
{
    _data.callbacks.push_back(trigger_callback);
    return *this;
}

template <typename... values_t>
const event<values_t...>& event<values_t...>::trigger(values_t&&... values) const
{
    detail::trigger(_data, std::forward<values_t>(values)...);
    return *this;
}

template <typename... values_t>
void event<values_t...>::trigger(std::future<future_values>&& future_value)
{
    detail::trigger(_data, std::move(future_value));
}

template <typename... values_t>
void event<values_t...>::trigger(const std::shared_future<future_values>& shared_future_value)
{
    detail::trigger(_data, shared_future_value);
}

template <typename... values_t>
std::vector<std::function<void(const values_t&...)>>& event<values_t...>::trigger_callbacks()
{
    return _data.callbacks;
}

// template <typename... values_t>
// const std::vector<std::function<void(const values_t&...)>>& event<values_t...>::trigger_callbacks() const
// {
//     return _data.callbacks;
// }
}