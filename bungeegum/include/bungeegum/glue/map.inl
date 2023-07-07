#pragma once

namespace bungeegum {
namespace detail {

    template <typename key_t, typename value_t>
    indexed_map<key_t, value_t>::indexed_map(std::initializer_list<std::pair<key_t, value_t>> values)
    {
        for (const std::pair<key_t, value_t>& _value : values) {
            emplace(_value.first, _value.second);
        }
    }

    template <typename key_t, typename value_t>
    value_t& indexed_map<key_t, value_t>::at(const key_t& key)
    {
        if (!contains(key)) {
            throw backtraced_exception("No value can be accessed because the "
                                       "provided key is not contained.");
        }
        std::size_t _index = _indices.at(key);
        return (_values[_index]);
    }

    template <typename key_t, typename value_t>
    const value_t& indexed_map<key_t, value_t>::at(const key_t& key) const
    {
        if (!contains(key)) {
            throw backtraced_exception("No value can be accessed because the "
                                       "provided key is not contained.");
        }
        std::size_t _index = _indices.at(key);
        return (_values[_index]);
    }

    template <typename key_t, typename value_t>
    indexed_map<key_t, value_t>::template iterator indexed_map<key_t, value_t>::begin()
    {
        return (_values.begin());
    }

    template <typename key_t, typename value_t>
    indexed_map<key_t, value_t>::template const_iterator indexed_map<key_t, value_t>::begin() const
    {
        return (_values.begin());
    }

    template <typename key_t, typename value_t>
    bool indexed_map<key_t, value_t>::contains(const key_t& key)
    {
        return (_indices.find(key) != _indices.end());
    }

    template <typename key_t, typename value_t>
    value_t& indexed_map<key_t, value_t>::emplace(const key_t& key, const value_t& value)
    {
        if (contains(key)) {
            throw backtraced_exception("No key-value pair can be emplaced because the "
                                       "provided key is already contained.");
        }
        std::size_t _index = _values.size();
        _indices.emplace(key, _index);
        return (_values.emplace_back(value));
    }

    template <typename key_t, typename value_t>
    void indexed_map<key_t, value_t>::erase(const key_t& key)
    {
        if (!contains(key)) {
            throw backtraced_exception("No key-value pair can be erased because the "
                                       "provided key is not contained.");
        }
        std::size_t _index = _indices.at(key);
        _values.erase(_values.begin() + _index);
        _indices.erase(key);
    }

    template <typename key_t, typename value_t>
    indexed_map<key_t, value_t>::template const_iterator indexed_map<key_t, value_t>::end() const
    {
        return (_values.end());
    }

    template <typename key_t, typename value_t>
    indexed_map<key_t, value_t>::template iterator indexed_map<key_t, value_t>::end()
    {
        return (_values.end());
    }

    template <typename key_t, typename value_t>
    value_t& indexed_map<key_t, value_t>::operator[](const key_t& key)
    {
        static_assert(std::is_default_constructible_v<value_t>, "The type value_t must be default-constructible to use the "
                                                                "operator[] method, because a default-constructed value must "
                                                                "be created when the key is not contained.");
        if (!contains(key)) {
            emplace(key, value_t {});
        }
        std::size_t _index = _indices.at(key);
        return (_values[_index]);
    }

    template <typename key_t, typename value_t>
    const value_t& indexed_map<key_t, value_t>::operator[](const key_t& key) const
    {
        static_assert(std::is_default_constructible_v<value_t>, "The type value_t must be default-constructible to use the "
                                                                "operator[] method, because a default-constructed value must "
                                                                "be created when the key is not contained.");
        if (!contains(key)) {
            emplace(key, value_t {});
        }
        std::size_t _index = _indices.at(key);
        return (_values[_index]);
    }

}
}