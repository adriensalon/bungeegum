//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <entt/entt.hpp>

#include <staticgui/glue/typelist.hpp>

namespace staticgui {
namespace glue {

    using entity = ENTT_ID_TYPE;

    struct generator {
        inline static entity create()
        {
            entity _retval = _max_entity;
            _max_entity++;
            return _retval;
        }
        inline static void destroy(const entity& ent)
        {
        }

    private:
        inline static entity _max_entity = 0;
        inline static std::vector<entity> _deleted_entities;
    };

    struct registry {
        registry();
        registry(const registry& other) = delete;
        registry& operator=(const registry& other) = delete;
        registry(registry&& other);
        registry& operator=(registry&& other);

        // create entity

        // destroy(const entity& entity);

        entity create_entity();

        template <typename component_t>
        entity get_entity(component_t& component)
        {
            return entt::to_entity(_registry, component);
        }

        template <typename component_t>
        [[nodiscard]] bool has_component(const entity entity) const;

        template <typename component_t, typename... args_t>
        decltype(auto) create_component(const entity entity, args_t&&... args);

        void destroy_entity_components(const entity ent);

        template <typename component_t>
        component_t& get_component(const entity entity);

        template <typename component_t>
        const component_t& get_component(const entity entity) const;

        // template <typename component_t>
        // [[nodiscard]] const component_t& get_component(const entity& entity) const;

        // template <typename component_t>
        // [[nodiscard]] component_t& get_or_create_component(const entity& entity);

        // template <typename component_t>
        // [[nodiscard]] const component_t& get_or_create_component(const entity& entity) const;

        // iterate entities only

        template <typename... components_t> // exclude filter
        void iterate_components(const std::function<void(components_t&...)>& iterate_function);

        template <typename... components_t> // exclude filter
        void iterate_entities_components(const std::function<void(entity, components_t&...)>& iterate_function);

    private:
        entt::basic_registry<entity> _registry;
    };

}
}

#include <staticgui/glue/registry.inl>