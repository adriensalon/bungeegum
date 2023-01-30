//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>
#include <memory>
#include <optional>

#include <entt/entt.hpp>
#include <tree.hh>

#include <staticgui/glue/meta.hpp>

namespace staticgui {
namespace glue {
    namespace containers {

        struct any_function {
            any_function() noexcept;

            any_function(nullptr_t) noexcept;

            any_function(const any_function& other);

            any_function& operator=(const any_function& other);

            any_function(any_function&& other) noexcept;

            any_function& operator=(any_function&& other) noexcept;

            template <typename return_t, typename... args_t>
            any_function& operator=(const std::function<return_t(args_t...)>& function);

            template <typename return_t, typename... args_t>
            std::function<return_t(args_t...)>& get();

            template <typename return_t, typename... args_t>
            return_t operator()(args_t&&... args);

        private:
            std::any _untyped;
            std::shared_ptr<std::type_index> _typeindex;
        };

        using entity = entt::entity;

        struct registry {
            registry();

            registry(const registry& other);

            registry& operator=(const registry& other);

            registry(registry&& other);

            registry& operator=(registry&& other);

            // create entity

            // destroy(const entity& ent);

            template <typename component_t>
            [[nodiscard]] bool has_component(const entity& ent) const;

            template <typename component_t, typename... args_t>
            component_t& create_component(const entity& ent, args_t&&... args);

            // destroy component

            template <typename component_t>
            [[nodiscard]] component_t& get_component(const entity& ent);

            template <typename component_t>
            [[nodiscard]] const component_t& get_component(const entity& ent) const;

            template <typename component_t>
            [[nodiscard]] component_t& get_or_create_component(const entity& ent);

            template <typename component_t>
            [[nodiscard]] const component_t& get_or_create_component(const entity& ent) const;

            // iterate entities only

            template <typename... components_t>
            void iterate_components(std::function<void(components_t&...)> iterate_function);

            template <typename... components_t, typename function_t>
            void iterate_entities_components(function_t&& iterate_function);

        private:
            std::shared_ptr<entt::registry> _registry;
            // friend struct entity;
        };

        template <typename node_t, typename node_allocator_t = std::allocator<tree_node_<node_t>>>
        struct tree {
        };

        template <typename node_t>
        struct tree<std::reference_wrapper<node_t>> {
        };

    }
}
}

#include <staticgui/glue/containers.inl>