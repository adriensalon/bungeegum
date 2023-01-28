//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <memory>
#include <optional>

#include <entt/entt.hpp>

#include <staticgui/utils/typelist.hpp>

namespace staticgui {
namespace internal {
    namespace ecs {

        struct entity;

        struct registry {

            registry();

            registry(const registry& other);

            registry& operator=(const registry& other);

            registry(registry&& other);

            registry& operator=(registry&& other);

            // entity create();

            // destroy(const entity& ent);

            // iterate entities only

            template <typename... components_t>
            void iterate_components(std::function<void(components_t&...)> iterate_function);

            template <typename... components_t, typename function_t>
            void iterate_entities_components(function_t&& iterate_function);

        private:
            std::shared_ptr<entt::registry> _registry;
            friend struct entity;
        };

        struct entity {

            entity(registry& reg);

            entity(registry& reg, const entt::entity raw_ent);

            entity(const entity& other);

            entity& operator=(const entity& other);

            entity(entity&& other);

            entity& operator=(entity&& other);

            template <typename component_t>
            [[nodiscard]] bool has_component() const;

            template <typename component_t, typename... args_t>
            component_t& create_component(args_t&&... args);

            template <typename component_t>
            [[nodiscard]] component_t& get_component();

            template <typename component_t>
            [[nodiscard]] const component_t& get_component() const;

            template <typename component_t>
            [[nodiscard]] component_t& get_or_create_component();

            template <typename component_t>
            [[nodiscard]] const component_t& get_or_create_component() const;

            // private:
            entt::entity _entity;
            std::shared_ptr<entt::registry> _registry;
        };

    }
}
}

#include <staticgui/utils/ecs.inl>