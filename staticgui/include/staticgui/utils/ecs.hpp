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

#include <staticgui/utils/typelist.hpp>

namespace staticgui {
namespace internal {
    namespace ecs {

        template <typename value_t>
        struct registry;

        template <typename value_t>
        struct entity;

        template <typename... tags_t>
        struct tags_filter : public typelist::strong_typelist<tags_filter_type, tags_t...> {
        };

        template <typename... tags_ts>
        struct tags_exclude_filter : public typelist::strong_typelist<tags_exclude_filter, tags_ts...> {
        };

        template <typename value_t>
        struct registry {

            registry();

            registry(const registry& other) = delete;

            registry& operator=(const registry& other) = delete;

            registry(registry&& other);

            registry& operator=(registry&& other);

            entity<value_t>& create_entity();

            template <typename... tags_t, typename... exclude_tags_t, typename... components_t>
            void iterate_components(tags_filter_type<tags_t...> tags, tags_exclude_filter<exclude_tags_t...> exclude_tags, std::function<void(components_t&...)> iterate_function);

            template <typename... tags_t, typename... exclude_tags_t, typename... components_t>
            void iterate_entities_and_components(tags_filter_type<tags_t...> tags, tags_exclude_filter<exclude_tags_t...> exclude_tags, std::function<void(entity<value_t>&, components_t&...)> iterate_function);

        private:
            entt::registry _registry;
        };

        template <typename value_t>
        struct entity {

            entity(const registry<value_t>& reg);

            entity(const entity& other) = delete;

            entity& operator=(const entity& other) = delete;

            entity(entity&& other);

            entity& operator=(entity&& other);

            [[nodiscard]] value_t& get_data();

            [[nodiscard]] bool is_enabled() const;

            registry<value_t>& get_registry() const;

            template <typename component_t>
            [[nodiscard]] bool has_component() const;

            template <typename component_t>
            component_t& create_component();

            template <typename component_t>
            [[nodiscard]] component_t& get_component();

            template <typename component_t>
            [[nodiscard]] const component_t& get_component() const;

            template <typename component_t>
            [[nodiscard]] component_t& get_or_create_component();

            template <typename component_t>
            [[nodiscard]] const component_t& get_or_create_component() const;

        private:
            registry<value_t>& _registry;
            entt::entity _entity;
        };
    }
}
}

#include <staticgui/utils/ecs.inl>