//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <variant>

namespace bungeegum {

template <typename component_t>
entity_t registry::get_entity(component_t& component)
{
    return entt::to_entity(_registry, component);
}

template <typename component_t>
std::optional<entity_t> registry::try_get_entity(component_t& component)
{
    // maintain an std::unordered_set ?
    std::optional<entity_t> _retval = std::nullopt;
    iterate<component_t>([this, &component, &_retval](entity_t _entity, component_t& _component) {
        if (&_component == &component)
            _retval = _entity;
    });
    return _retval;
}

// TODO has component

template <typename component_t, typename... args_t>
decltype(auto) registry::create_component(const entity_t entity, args_t&&... args)
{
    return _registry.emplace<component_t>(entity, std::forward<args_t>(args)...);
}

template <typename component_t>
component_t& registry::get_component(const entity_t entity)
{
    return _registry.get<component_t>(entity);
}

template <typename component_t>
const component_t& registry::get_component(const entity_t entity) const
{
    return _registry.view<component_t>().get<component_t>(entity);
}

template <typename... components_t>
void registry::iterate(const std::function<void(components_t&...)>& iterate_function)
{
    auto& _view = _registry.view<components_t...>();
    _view.each([&](components_t&... _components) {
        iterate_function(_components...);
    });
}

template <typename... components_t>
void registry::iterate(const std::function<void(entity_t, components_t&...)>& iterate_function)
{
    auto& _view = _registry.view<components_t...>();
    _view.each([&](entity_t _entity, components_t&... _components) {
        iterate_function(_entity, _components...);
    });
}

}