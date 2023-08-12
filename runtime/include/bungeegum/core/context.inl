#pragma once

#include <bungeegum/core/global.fwd>

namespace bungeegum {

template <typename value_t>
bool is_contextual(const std::string& name)
{
    std::unordered_map<std::string, std::any>& _map = detail::global().context.untyped_map;
    if (_map.find(name) == _map.end()) {
        return false;
    }
    return _map.at(name).type() == typeid(value_t);
}

template <typename value_t>
const value_t& get_contextual(const std::string& name)
{
    std::unordered_map<std::string, std::any>& _map = detail::global().context.untyped_map;
    return std::any_cast<value_t>(_map.at(name));
}

template <typename value_t>
void set_contextual(const std::string& name, const value_t& value)
{
    std::unordered_map<std::string, std::any>& _map = detail::global().context.untyped_map;
    _map[name] = std::make_any<value_t>(value);
}

template <typename value_t>
void set_contextual(const std::string& name, value_t&& value)
{
    std::unordered_map<std::string, std::any>& _map = detail::global().context.untyped_map;
    _map[name] = std::make_any<value_t>(std::move(value));
}

}