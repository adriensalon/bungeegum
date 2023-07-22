#pragma once

#include <variant>

namespace bungeegum {
namespace detail {

    template <typename entity_t>
    entity_t registry<entity_t>::create_entity()
    {
        return _registry.create();
    }

    template <typename entity_t>
    void registry<entity_t>::erase(const entity_t entity)
    {
        _registry.destroy(entity);
    }

    template <typename entity_t>
    template <typename component_t, typename... args_t>
    decltype(auto) registry<entity_t>::create_component(const entity_t entity, args_t&&... args)
    {
        return _registry.template emplace<component_t>(entity, std::forward<args_t>(args)...);
    }

    template <typename entity_t>
    template <typename component_t>
    component_t& registry<entity_t>::get_component(const entity_t entity)
    {
        return _registry.template get<component_t>(entity);
    }

    template <typename entity_t>
    template <typename component_t>
    const component_t& registry<entity_t>::get_component(const entity_t entity) const
    {
        return _registry.template view<component_t>().template get<component_t>(entity);
    }

    template <typename entity_t>
    template <typename... components_t>
    void registry<entity_t>::iterate(const std::function<void(components_t&...)>& iterate_function)
    {
        auto _view = _registry.template view<components_t...>();
        _view.each([&](components_t&... _components) {
            iterate_function(_components...);
        });
    }

    template <typename entity_t>
    template <typename... components_t>
    void registry<entity_t>::iterate_with_entities(const std::function<void(entity_t, components_t&...)>& iterate_function)
    {
        auto _view = _registry.template view<components_t...>();
        _view.each([&](entity_t _entity, components_t&... _components) {
            iterate_function(_entity, _components...);
        });
    }

    template <typename entity_t>
    std::size_t registry<entity_t>::size() const
    {
        return _registry.size();
    }
}
}