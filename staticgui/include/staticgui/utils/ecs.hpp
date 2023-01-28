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

#include <entt/entt.hpp>

#include <staticgui/utils/typelist.hpp>

namespace staticgui {
namespace internal {
    namespace ecs {

        struct registry;
        struct entity;

        // template <typename... tags_t>
        // struct tags_filter : public typelist::strong_typelist<tags_filter, tags_t...> {
        // };

        // template <typename... tags_ts>
        // struct tags_exclude_filter : public typelist::strong_typelist<tags_filter, tags_ts...> {
        // };

        struct registry {

            registry();

            registry(const registry& other) = delete;

            registry& operator=(const registry& other) = delete;

            registry(registry&& other);

            registry& operator=(registry&& other);

            // iterate entities

            template <typename... components_t>
            void iterate_components(std::function<void(components_t&...)> iterate_function);

            // template <typename... components_t>
            template <typename... components_t, typename function_t>
            void iterate_entities_components(function_t&& iterate_function);

        private:
            entt::registry _registry;
            friend struct entity;
        };

        struct entity {

            // GET ID !! avec static_cast izi

            entity();

            bool is_valid() const;

            entity(registry& reg);

            entity(registry& reg, const entt::entity raw_ent);

            entity(const entity& other);

            entity& operator=(const entity& other);

            entity(entity&& other) = delete;

            entity& operator=(entity&& other) = delete;

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
            bool _is_valid;
            entt::entity _entity;
            entt::registry* _registry_ptr;
        };

    }
}
}

#include <staticgui/utils/ecs.inl>