#pragma once

#include <algorithm>

#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {    

    template <typename... values_t>
    void assign_ticker(event_data<values_t...>& data)
    {
        using callback_iterator = typename event<values_t...>::on_trigger_callback;
        using future_iterator = typename std::vector<std::future<typename event<values_t...>::future_values>>::iterator;
        using shared_future_iterator = typename std::vector<std::shared_future<typename event<values_t...>::future_values>>::iterator;
        
        data.update_data.ticker = [&data](events_manager_data& manager_data) {

            for (future_iterator _future_it = data.futures.begin(); _future_it != data.futures.end();) {
                if (_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    if constexpr (variadic_reduce<values_t...>::is_void()) {
                        for (callback_iterator _callback_it : data.callbacks)
                            _callback_it();
                    } else {
                        variadic_reduce_t<values_t...> _vals = _future_it->get();
                        if constexpr (variadic_reduce<values_t...>::is_tuple()) {
                            for (callback_iterator _callback_it : data.callbacks)
                                _callback_it(std::forward<values_t>(std::get<values_t>(_vals))...);
                        } else {
                            for (callback_iterator _callback_it : data.callbacks)
                                _callback_it(std::forward<values_t>(_vals)...);
                        }
                    }
                    _future_it = data.futures.erase(_future_it);                    
                } else
                    _future_it++;
            }

            for (shared_future_iterator _shared_future_it = data.shared_futures.begin(); _shared_future_it != data.shared_futures.end();) {
                if (_shared_future_it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                    if constexpr (variadic_reduce<values_t...>::is_void()) {
                        for (callback_iterator _callback : data.callbacks)
                            _callback();
                    } else {
                        variadic_reduce_t<values_t...> _vals = _shared_future_it->get();
                        if constexpr (variadic_reduce<values_t...>::is_tuple()) {
                            for (callback_iterator _callback : data.callbacks)
                                _callback(std::forward<values_t>(std::get<values_t>(_vals))...);
                        } else {
                            for (callback_iterator _callback : data.callbacks)
                                _callback(std::forward<values_t>(_vals)...);
                        }
                    }
                    _shared_future_it = data.shared_futures.erase(_shared_future_it);
                } else
                    _shared_future_it++;
            }
            
            if (data.futures.empty() && data.shared_futures.empty()) {
                manager_data.updatables_to_erase.push_back(data.raw);
                return;
            }
        };
    }

    template <typename... values_t>
    event_data<values_t...>::event_data()
    {        
        raw = raw_cast(this); // create a new id
        (update_data.kinds.push_back(typeid(values_t)), ...);
    }

    template <typename... values_t>
    event_data<values_t...>::event_data(const event_data& other)
    {
        *this = other;
    }

    template <typename... values_t>
    event_data<values_t...>& event_data<values_t...>::operator=(const event_data<values_t...>& other)
    {
        raw = raw_cast(this); // create a new id
        update_data.kinds = other.update_data.kinds;
        callbacks = other.callbacks;
        return *this;
    }

    template <typename... values_t>
    event_data<values_t...>::event_data(event_data&& other)
    {
        *this = std::move(other);
    }

    template <typename... values_t>
    event_data<values_t...>& event_data<values_t...>::operator=(event_data<values_t...>&& other)
    {
        raw = other.raw; // keep the same id
        callbacks = std::move(other.callbacks);
        futures = std::move(other.futures);
        shared_futures = std::move(other.shared_futures);
        (_data.update_data.kinds.push_back(typeid(values_t)), ...);
        global_manager_data& _global = global();
        if (_global.events.updatables.find(raw) != _global.events.updatables.end()) {
            _global.events.updatables.at(raw) = std::ref(update_data);
            assign_ticker(*this);
        }
        return *this;
    }
    
    template <typename... values_t>
    event_data<values_t...>::~event_data()
    {
        global_manager_data& _global = global();
        if (_global.events.updatables.find(raw) != _global.events.updatables.end()) {
            _global.events.updatables_to_erase.push_back(raw);
        }
    }
}

template <typename... values_t>
std::vector<std::function<void(const values_t&...)>>& event<values_t...>::get_callbacks()
{
    return _data.callbacks;
}

template <typename... values_t>
const std::vector<std::function<void(const values_t&...)>>& event<values_t...>::get_callbacks() const
{
    return _data.callbacks;
}

// is waiting

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
    detail::global_manager_data& _global = detail::global();
    if (_global.events.updatables.find(_data.raw) != _global.events.updatables.end()) {
        // error because event is being waited on already
    }
    _global.events.updatables.insert({ _data.raw, std::ref(_data.update_data) });
    detail::assign_ticker(_data);
#if BUNGEEGUM_USE_OVERLAY
        // for (const std::type_index& _type_index : _update_data.kinds) {
        //     _update_data.clean_typenames.push_back(detail::pipelines_manager::to_clean_typename(_type_index.name()));
        // }
#endif
    _data.futures.push_back(std::move(future_value));
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::trigger(const std::shared_future<future_values>& shared_future_value)
{
    detail::global_manager_data& _global = detail::global();
    if (_global.events.updatables(_data.raw) != _global.events.updatables.end()) {
        // error because event is being waited on already
    }
    _global.events.updatables.insert({ _data.raw, std::ref(_data.update_data) });
    detail::assign_ticker(_data);
#if BUNGEEGUM_USE_OVERLAY
        // for (const std::type_index& _type_index : _update_data.kinds) {
        //     _update_data.clean_typenames.push_back(detail::global().pipelines.to_clean_typename(_type_index.name()));
        // }
#endif
    _data.shared_futures.push_back(shared_future_value);
    return *this;
}
}