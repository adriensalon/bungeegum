#pragma once

#include <algorithm>

namespace bungeegum {
namespace detail {

    // typed_event_data

    template <typename... values_t>
    typed_event_data<values_t...>::typed_event_data(const typed_event_data& other)
    {
        *this = other;
    }

    template <typename... values_t>
    typed_event_data<values_t...>& typed_event_data<values_t...>::operator=(const typed_event_data<values_t...>& other)
    {
        callbacks = std::move(other.callbacks);
        return *this;
    }

    template <typename... values_t>
    typed_event_data<values_t...>::~typed_event_data()
    {
        if (is_event_possessed(raw_event))
            events_context.possessed.erase(raw_event);
        if (is_event_registered(raw_event))
            events_context.registered.erase(raw_event);
    }

    // events_registry

    template <typename... values_t>
    typed_event_data<values_t...>& events_registry::get_typed(event<values_t...>& event_object)
    {
        return event_object._data;
    }

    // free

    enum event_raw_access_mode {
        event_recast,
        event_stored
    };

    template <event_raw_access_mode access_mode_t, typename... values_t>
    std::uintptr_t get_raw_event(event<values_t...>& event_object)
    {
        if constexpr (access_mode_t == event_raw_access_mode::event_recast)
            return raw_cast(event_object);
        else if constexpr (access_mode_t == event_raw_access_mode::event_stored)
            return events_context.get_typed<values_t...>(event_object).raw_event;
    }

    inline bool is_event_registered(const std::uintptr_t raw_event)
    {
        return events_context.registered.find(raw_event) != events_context.registered.end();
    }

    template <event_raw_access_mode access_mode_t, typename... values_t>
    bool is_event_registered(event<values_t...>& event_object)
    {
        std::uintptr_t _raw_event = get_raw_event<access_mode_t, values_t...>(event_object);
        return is_event_registered(_raw_event);
    }

    inline bool is_event_possessed(const std::uintptr_t raw_event)
    {
        return events_context.possessed.find(raw_event) != events_context.possessed.end();
    }

    template <event_raw_access_mode access_mode_t, typename... values_t>
    bool is_event_possessed(event<values_t...>& event_object)
    {
        std::uintptr_t _raw_event = get_raw_event<access_mode_t, values_t...>(event_object);
        return is_event_possessed(_raw_event);
    }

    template <typename... values_t>
    void assign_on_tick(typed_event_data<values_t...>& typed_event, untyped_event_data& untyped_event)
    {
        using callback_iterator = typename event<values_t...>::on_trigger_callback;
        using future_iterator = typename std::vector<std::future<event<values_t...>::future_values>>::iterator;
        using shared_future_iterator = typename std::vector<std::shared_future<event<values_t...>::future_values>>::iterator;
        untyped_event.ticker = [&]() {
            for (future_iterator _future_it = typed_event.futures.begin(); _future_it != typed_event.futures.end();) {
                if (_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    if constexpr (future_typelist<values_t...>::is_void()) {
                        for (callback_iterator _callback_it : typed_event.callbacks)
                            _callback_it();
                    } else {
                        future_typelist_t<values_t...> _vals = _future_it->get();
                        if constexpr (future_typelist<values_t...>::is_tuple()) {
                            for (callback_iterator _callback_it : typed_event.callbacks)
                                _callback_it(std::forward<values_t>(std::get<values_t>(_vals))...);
                        } else {
                            for (callback_iterator _callback_it : typed_event.callbacks)
                                _callback_it(std::forward<values_t>(_vals)...);
                        }
                    }
                    _future_it = typed_event.futures.erase(_future_it);
                } else
                    _future_it++;
            }
            for (shared_future_iterator _shared_future_it = typed_event.shared_futures.begin(); _shared_future_it != typed_event.shared_futures.end();) {
                if (_shared_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {

                    if constexpr (future_typelist<values_t...>::is_void()) {
                        for (callback_iterator _callback : typed_event.callbacks)
                            _callback();
                    } else {
                        future_typelist_t<values_t...> _vals = _shared_future_it->get();
                        if constexpr (future_typelist<values_t...>::is_tuple()) {
                            for (callback_iterator _callback : typed_event.callbacks)
                                _callback(std::forward<values_t>(std::get<values_t>(_vals))...);
                        } else {
                            for (callback_iterator _callback : typed_event.callbacks)
                                _callback(std::forward<values_t>(_vals)...);
                        }
                    }
                    _shared_future_it = typed_event.shared_futures.erase(_shared_future_it);
                } else
                    _shared_future_it++;
            }
        };
    }

    template <typename... values_t>
    void assign_event(event<values_t...>& event_object, untyped_event_data& untyped_event, std::uintptr_t raw_event)
    {
        typed_event_data<values_t...>& _typed_event = events_context.get_typed<values_t...>(event_object);
        _typed_event.raw_event = raw_event;
        untyped_event.kinds = {};
        (untyped_event.kinds.push_back(typeid(values_t)), ...);
        events_context.registered.insert_or_assign(raw_event, std::ref(untyped_event));
        assign_on_tick(_typed_event, untyped_event);
    }

    template <typename... values_t>
    void register_event(event<values_t...>& event_object, const std::uintptr_t raw_event)
    {
        registry_entity _entity;
        if (is_event_possessed(raw_event)) {
            _entity = events_context.possessed.at(raw_event);
            untyped_event_data& _untyped_event = events_context.events.get_component<untyped_event_data>(_entity);
            assign_event(event_object, _untyped_event, raw_event);
            return;
        }
        // else {
        //     std::optional<registry_entity> _existing_entity = events_context.events.try_get_entity(event_object);
        //     if (_existing_entity != std::nullopt) {
        //         _entity = _existing_entity.value();
        //         untyped_event_data& _untyped_event = events_context.events.get_component<untyped_event_data>(_entity);
        //         assign_event(event_object, _untyped_event, raw_event);
        //         return;
        //     }
        // }
        _entity = events_context.events.create_entity();
        events_context.events.create_component<std::reference_wrapper<event<values_t...>>>(_entity, event_object);
        untyped_event_data& _untyped_event = events_context.events.create_component<untyped_event_data>(_entity);
        assign_event(event_object, _untyped_event, raw_event);
    }
}

// event

template <typename... values_t>
event<values_t...>& event<values_t...>::merge(const event<values_t...>& other)
{
    _data.callbacks.insert(_data.callbacks.end(),
        other._data.callbacks.begin(),
        other._data.callbacks.end());
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::merge(event<values_t...>&& other)
{
    _data.callbacks.insert(_data.callbacks.end(),
        std::make_move_iterator(other._data.callbacks.begin()),
        std::make_move_iterator(other._data.callbacks.end()));
    _data.futures.insert(_data.futures.end(),
        std::make_move_iterator(other._data.futures.begin()),
        std::make_move_iterator(other._data.futures.end()));
    _data.shared_futures.insert(_data.shared_futures.end(),
        std::make_move_iterator(other._data.shared_futures.begin()),
        std::make_move_iterator(other._data.shared_futures.end()));
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::on_trigger(const on_trigger_callback& trigger_callback)
{
    _data.callbacks.push_back(trigger_callback);
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::trigger(values_t&&... values) const
{
    for (on_trigger_callback& _callback : _data.callbacks)
        _callback(std::forward<values_t>(values)...);
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::trigger(std::future<future_values>&& future_value)
{
    std::uintptr_t _raw_event = detail::get_raw_event<detail::event_raw_access_mode::event_recast>(*this);
    if (!detail::is_event_registered(_raw_event))
        detail::register_event(*this, _raw_event);
    _data.futures.push_back(std::move(future_value));
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::trigger(const std::shared_future<future_values>& shared_future_value)
{
    std::uintptr_t _raw_event = detail::get_raw_event<detail::event_raw_access_mode::event_recast>(*this);
    if (!detail::is_event_registered(_raw_event))
        detail::register_event(*this, _raw_event);
    _data.shared_futures.push_back(shared_future_value);
    return *this;
}

template <typename... values_t>
std::vector<std::function<void(const values_t&...)>>& event<values_t...>::callbacks()
{
    return _data.callbacks;
}

template <typename... values_t>
const std::vector<std::function<void(const values_t&...)>>& event<values_t...>::callbacks() const
{
    return _data.callbacks;
}

// free

template <typename... values_t>
event<values_t...>& make_event()
{
    detail::registry_entity _entity = detail::events_context.events.create_entity();
    detail::events_context.events.create_component<detail::untyped_event_data>(_entity);
    event<values_t...>& _event = detail::events_context.events.create_component<event<values_t...>>(_entity);
    std::uintptr_t _raw_event = detail::get_raw_event<detail::event_raw_access_mode::event_recast>(_event);
    detail::events_context.possessed.emplace(_raw_event, _entity);
    return _event;
}

template <typename... values_t>
event<values_t...>& make_event(const event<values_t...>& other_event)
{
    detail::registry_entity _entity = detail::events_context.events.create_entity();
    detail::events_context.events.create_component<detail::untyped_event_data>(_entity);
    event<values_t...>& _event = detail::events_context.events.create_component<event<values_t...>>(_entity, other_event);
    std::uintptr_t _raw_event = detail::get_raw_event<detail::event_raw_access_mode::event_recast>(_event);
    detail::events_context.possessed.emplace(_raw_event, _entity);
    return _event;
}

template <typename... values_t>
event<values_t...>& make_event(event<values_t...>&& other_event)
{
    detail::registry_entity _entity = detail::events_context.events.create_entity();
    detail::events_context.events.create_component<detail::untyped_event_data>(_entity);
    event<values_t...>& _event = detail::events_context.events.create_component<event<values_t...>>(_entity, std::move(other_event));
    std::uintptr_t _raw_event = detail::get_raw_event<detail::event_raw_access_mode::event_recast>(_event);
    detail::events_context.possessed.emplace(_raw_event, _entity);
    return _event;
}

template <typename... values_t>
void unmake_event(event<values_t...>& made_event)
{
    std::uintptr_t _raw_event = detail::get_raw_event<detail::event_raw_access_mode::event_stored>(made_event);
    detail::registry_entity _entity = detail::events_context.possessed.at(_raw_event);
    detail::events_context.events.destroy_entity(_entity);
}
}