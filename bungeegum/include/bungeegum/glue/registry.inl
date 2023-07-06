#pragma once

#include <variant>

namespace bungeegum {
namespace detail {

    // template <typename component_t>
    // registry_entity registry::get_entity(component_t& component)
    // {
    //     // maintain an std::unordered_set ?
    //     return entt::to_entity(_registry, component);
    // }

    // template <typename component_t>
    // std::optional<registry_entity> registry::try_get_entity(component_t& component)
    // {
    //     // maintain an std::unordered_set ?
    //     std::optional<registry_entity> _retval = std::nullopt;
    //     iterate_with_entities<component_t>([this, &component, &_retval](registry_entity _entity, component_t& _component) {
    //         if (&_component == &component)
    //             _retval = _entity;
    //     });
    //     return _retval;
    // }

    // TODO has component

    template <typename component_t, typename... args_t>
    decltype(auto) registry::create_component(const registry_entity entity, args_t&&... args)
    {
        return _registry.template emplace<component_t>(entity, std::forward<args_t>(args)...);
    }

    template <typename component_t>
    component_t& registry::get_component(const registry_entity entity)
    {
        return _registry.template get<component_t>(entity);
    }

    template <typename component_t>
    const component_t& registry::get_component(const registry_entity entity) const
    {
        return _registry.template view<component_t>().template get<component_t>(entity);
    }

    template <typename... components_t>
    void registry::iterate(const std::function<void(components_t&...)>& iterate_function)
    {
        auto _view = _registry.template view<components_t...>();
        _view.each([&](components_t&... _components) {
            iterate_function(_components...);
        });
    }

    template <typename... components_t>
    void registry::iterate_with_entities(const std::function<void(registry_entity, components_t&...)>& iterate_function)
    {
        auto _view = _registry.template view<components_t...>();
        _view.each([&](registry_entity _entity, components_t&... _components) {
            iterate_function(_entity, _components...);
        });
    }

}
}