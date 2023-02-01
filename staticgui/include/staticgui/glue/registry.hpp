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

#include <staticgui/glue/id_integer.hpp>
#include <staticgui/glue/typelist.hpp>

namespace staticgui {
namespace glue {

    struct registry {
        registry();
        registry(const registry& other) = delete;
        registry& operator=(const registry& other) = delete;
        registry(registry&& other);
        registry& operator=(registry&& other);

        // create id_integer

        // destroy(const id_integer& entity);

        id_integer create_entity();

        template <typename component_t>
        id_integer get_entity(component_t& component)
        {
            return entt::to_entity(_registry, component);
        }

        template <typename component_t>
        [[nodiscard]] bool has_component(const id_integer entity) const;

        template <typename component_t, typename... args_t>
        decltype(auto) create_component(const id_integer entity, args_t&&... args);

        // // destroy component

        template <typename component_t>
        component_t& get_component(const id_integer entity);

        template <typename component_t>
        const component_t& get_component(const id_integer entity) const;

        // template <typename component_t>
        // [[nodiscard]] const component_t& get_component(const id_integer& entity) const;

        // template <typename component_t>
        // [[nodiscard]] component_t& get_or_create_component(const id_integer& entity);

        // template <typename component_t>
        // [[nodiscard]] const component_t& get_or_create_component(const id_integer& entity) const;

        // iterate entities only

        template <typename... components_t> // exclude filter
        void iterate_components(const std::function<void(components_t&...)>& iterate_function);

        template <typename... components_t> // exclude filter
        void iterate_entities_components(const std::function<void(id_integer, components_t&...)>& iterate_function);

    private:
        entt::basic_registry<id_integer> _registry;
    };

}
}

#include <staticgui/glue/registry.inl>