#pragma once

#include <type_traits>

#include <bungeegum/glue/raw.fwd>

namespace bungeegum {
namespace detail {

template <typename value_t, typename watcher_t>
value_contract<value_t, watcher_t>::value_contract()
{
    static_assert(std::is_default_constructible_v<value_t>, "Error");
    static_assert(std::is_default_constructible_v<watcher_t>, "Error");
    _raw_data_ptr = raw_cast(&value);
}

template <typename value_t, typename watcher_t>
value_contract<value_t, watcher_t>::value_contract(const value_contract& other)
{
    static_assert(std::is_copy_constructible_v<value_t>, "Error");
    static_assert(std::is_copy_constructible_v<watcher_t>, "Error");
    *this = other;
}

template <typename value_t, typename watcher_t>
value_contract<value_t, watcher_t>& value_contract<value_t, watcher_t>::operator=(const value_contract& other)
{
    static_assert(std::is_copy_assignable_v<value_t>, "Error");
    static_assert(std::is_copy_assignable_v<watcher_t>, "Error");
    value = other.value;
    _raw_data_ptr = raw_cast(&value);
    _watch_map = other._watch_map;
    if (is_watched()) {
        watcher_t& _updater = _watch_map.value().get().operator[](other._raw_data_ptr); // emplace
        _watch_map.value().get().operator[](_raw_data_ptr) = _updater; // copy
    }
    return *this;
}

template <typename value_t, typename watcher_t>
value_contract<value_t, watcher_t>::value_contract(value_contract&& other)
{
    static_assert(std::is_move_constructible_v<value_t>, "Error");
    static_assert(std::is_move_constructible_v<watcher_t>, "Error");
    *this = std::move(other);
}

template <typename value_t, typename watcher_t>
value_contract<value_t, watcher_t>& value_contract<value_t, watcher_t>::operator=(value_contract&& other)
{
    static_assert(std::is_move_assignable_v<value_t>, "Error");
    static_assert(std::is_move_assignable_v<watcher_t>, "Error");
    value = std::move(other.value);
    _raw_data_ptr = raw_cast(&value);
    _watch_map = std::move(other._watch_map);
    if (is_watched()) {
        watcher_t& _updater = _watch_map.value().get().operator[](other._raw_data_ptr); // emplace
        _watch_map.value().get().operator[](_raw_data_ptr) = std::move(_updater); // move
        other.unwatch(); // invalidate
    }
    return *this;
}

template <typename value_t, typename watcher_t>
value_contract<value_t, watcher_t>::~value_contract()
{
    if (is_watched()) {
        unwatch();
    }
}

template <typename value_t, typename watcher_t>
bool value_contract<value_t, watcher_t>::is_watched() const
{
    return _watch_map.has_value();
}

template <typename value_t, typename watcher_t>
void value_contract<value_t, watcher_t>::unwatch()
{
    if (!is_watched()) {
        // throw
    }
    _watch_map.value().get().erase(_raw_data_ptr); // erase
    _watch_map = std::nullopt;
}

template <typename value_t, typename watcher_t>
void value_contract<value_t, watcher_t>::watch(value_contract<value_t, watcher_t>::watch_map_t& map) // give map here
{
    // static assert watcher has method setup(data_t&)
    if (is_watched()) {
        // throw
    }
    _watch_map = map;
    watcher_t& _updater = _watch_map.value().get().operator[](_raw_data_ptr);
    // _updater.dosomethingtosetup(*this); // go detect
    _updater.callback = [](value_t& v) {
        std::cout << "Callback !" << std::endl;
		(void)v;
    };
}

template <typename value_t, typename watcher_t>
ptr_contract<value_t, watcher_t>::ptr_contract()
{
    static_assert(!std::is_pointer_v<value_t>, "Error");
    static_assert(std::is_default_constructible_v<watcher_t>, "Error");
}

template <typename value_t, typename watcher_t>
ptr_contract<value_t, watcher_t>::ptr_contract(const ptr_contract& other)
{
    *this = other;
}

template <typename value_t, typename watcher_t>
ptr_contract<value_t, watcher_t>& ptr_contract<value_t, watcher_t>::operator=(const ptr_contract& other)
{
    ptr = other.ptr;
    return *this;
}

template <typename value_t, typename watcher_t>
ptr_contract<value_t, watcher_t>::ptr_contract(ptr_contract&& other)
{
    static_assert(std::is_move_constructible_v<watcher_t>, "Error");
    *this = std::move(other);
}

template <typename value_t, typename watcher_t>
ptr_contract<value_t, watcher_t>& ptr_contract<value_t, watcher_t>::operator=(ptr_contract&& other)
{
    ptr = std::move(other.ptr);
    other.ptr = nullptr;
    return *this;
}

template <typename value_t, typename watcher_t>
bool ptr_contract<value_t, watcher_t>::is_watched() const
{
    return ptr.has_value();
}

template <typename value_t, typename watcher_t>
void ptr_contract<value_t, watcher_t>::unwatch(watch_map_t& map)
{
    if (!is_watched()) {
        // throw
    }
    std::uintptr_t _raw_data_ptr = raw_cast(ptr);
    map.erase(_raw_data_ptr);
    ptr = nullptr;
}

template <typename value_t, typename watcher_t>
void ptr_contract<value_t, watcher_t>::watch(value_t& ref, watch_map_t& map)
{
    // static assert watcher has method setup(data_t&)
    if (is_watched()) {
        // throw
    }

    ptr = ref;
    std::uintptr_t _raw_data_ptr = raw_cast(&(ptr.value().get()));
    watcher_t& _updater = map.operator[](_raw_data_ptr);
    // _updater.dosomethingtosetup(*this); // go detect
    _updater.callback = []() {
        std::cout << "Callback 22!" << std::endl;
    };
}

}
}