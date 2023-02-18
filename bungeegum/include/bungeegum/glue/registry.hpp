//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <optional>
#include <vector>

#include <entt/entt.hpp>

#include <bungeegum/glue/typelist.hpp>

namespace bungeegum {

using entity_t = ENTT_ID_TYPE;

struct generator {
    inline static entity_t create()
    {
        entity_t _retval = _max_entity;
        _max_entity++;
        return _retval;
    }
    inline static void destroy(const entity_t& ent)
    {
    }

private:
    inline static entity_t _max_entity = 0;
    inline static std::vector<entity_t> _deleted_entities;
};

struct registry {
    registry();
    registry(const registry& other) = delete;
    registry& operator=(const registry& other) = delete;
    registry(registry&& other);
    registry& operator=(registry&& other);

    entity_t create_entity();
    void destroy_entity(const entity_t entity);

    template <typename component_t>
    std::optional<entity_t> try_get_entity(component_t& component);

    template <typename component_t>
    entity_t get_entity(component_t& component);

    template <typename component_t>
    [[nodiscard]] bool has_component(const entity_t entity) const;

    template <typename component_t, typename... args_t>
    decltype(auto) create_component(const entity_t entity, args_t&&... args);

    template <typename component_t>
    component_t& get_component(const entity_t entity);

    template <typename component_t>
    const component_t& get_component(const entity_t entity) const;

    template <typename... components_t> // exclude filter
    void iterate(const std::function<void(components_t&...)>& iterate_function);

    template <typename... components_t> // exclude filter
    void iterate(const std::function<void(entity_t, components_t&...)>& iterate_function);

private:
    entt::basic_registry<entity_t> _registry;
};

}

#include <bungeegum/glue/registry.inl>