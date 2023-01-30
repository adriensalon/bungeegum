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
#include <typeindex>

#include <entt/entt.hpp>
#include <tree.hh>

#include <staticgui/glue/id.hpp>
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

        // exclude filter
        template <typename... values_t> // https://skypjack.github.io/2019-04-12-entt-tips-and-tricks-part-1/
        struct group {
            group();
            group(const group& other);
            group& operator=(const group& other);
            group(group&& other);
            group& operator=(group&& other);

        private:
            entt::basic_group<id::integer, values_t...> _group;
        };

        struct registry {
            registry();
            registry(const registry& other) = delete;
            registry& operator=(const registry& other) = delete;
            registry(registry&& other);
            registry& operator=(registry&& other);

            // create id::integer

            // destroy(const id::integer& entity);

            template <typename component_t>
            [[nodiscard]] bool has_component(const id::integer& entity) const;

            template <typename component_t, typename... args_t>
            component_t& create_component(const id::integer& entity, args_t&&... args);

            // destroy component

            template <typename component_t>
            [[nodiscard]] component_t& get_component(const id::integer& entity);

            template <typename component_t>
            [[nodiscard]] const component_t& get_component(const id::integer& entity) const;

            template <typename component_t>
            [[nodiscard]] component_t& get_or_create_component(const id::integer& entity);

            template <typename component_t>
            [[nodiscard]] const component_t& get_or_create_component(const id::integer& entity) const;

            // iterate entities only

            template <typename... components_t> // exclude filter
            void iterate_components(const std::function<void(components_t&...)>& iterate_function);

            template <typename... components_t> // exclude filter
            void iterate_entities_components(const std::function<void(id::integer, components_t&...)>& iterate_function);

        private:
            entt::basic_registry<id::integer> _registry;
        };

        template <typename node_t, typename node_allocator_t = std::allocator<kbtree::tree_node_<node_t>>>
        struct tree {
            tree();
            tree(const node_t& head);
            tree(node_t&& head);
            tree(const tree& other);
            tree& operator=(const tree& other);
            tree(tree&& other);
            tree& operator=(tree&& other);
            ~tree();

            using pre_order_iterator = typename kbtree::tree<node_t, node_allocator_t>::pre_order_iterator;
            using post_order_iterator = typename kbtree::tree<node_t, node_allocator_t>::post_order_iterator;
            using sibling_iterator = typename kbtree::tree<node_t, node_allocator_t>::sibling_iterator;
            using leaf_iterator = typename kbtree::tree<node_t, node_allocator_t>::leaf_iterator;

        private:
            kbtree::tree<node_t, node_allocator_t> _tree;
        };

        template <typename node_t>
        struct tree<std::reference_wrapper<node_t>> {
            tree();
            tree(const std::reference_wrapper<node_t>& head);
            tree(std::reference_wrapper<node_t>&& head);
            tree(const tree& other);
            tree& operator=(const tree& other);
            tree(tree&& other);
            tree& operator=(tree&& other);
            ~tree();

            using pre_order_iterator = typename kbtree::tree<node_t*>::pre_order_iterator;
            using post_order_iterator = typename kbtree::tree<node_t*>::post_order_iterator;
            using sibling_iterator = typename kbtree::tree<node_t*>::sibling_iterator;
            using leaf_iterator = typename kbtree::tree<node_t*>::leaf_iterator;

        private:
            kbtree::tree<node_t*> _tree;
        };

    }
}
}

#include <staticgui/glue/containers.inl>