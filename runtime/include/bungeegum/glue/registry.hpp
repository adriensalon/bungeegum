#pragma once

#include <optional>
#include <vector>

#include <entt/entt.hpp>

#include <bungeegum/glue/typelist.hpp>

namespace bungeegum {
namespace detail {

    /// @brief Instances of this type represent arithmetic values to use as entities in registries.
    /// @details This type can be modified to use longer integers for example.
    using registry_entity = ENTT_ID_TYPE;

    /// @brief Instances of this struct represent type-erasure containers that allow fast memory
    /// management over entities and typed components.
    /// @details Instances of this struct can only be moved.
    /// @tparam entity_t
    template <typename entity_t = registry_entity>
    struct registry {
        registry() = default;
        registry(const registry& other) = delete;
        registry& operator=(const registry& other) = delete;
        registry(registry&& other) = default;
        registry& operator=(registry&& other) = default;

        /// @brief Creates a new entity in the registry.
        entity_t create_entity();

        /// @brief Destroys the specified entity from the registry.
        /// @exception Throws an entt exception if the specified entity is not contained by the
        /// registry.
        void erase(const entity_t entity);

        /// @brief Gets if the specified has a component of a certain type.
        /// @tparam component_t is the component type to check.
        template <typename component_t>
        [[nodiscard]] bool has_component(const entity_t entity) const;

        /// @brief Adds a new component to the specified entity.
        /// @tparam component_t is the component type to create.
        /// @tparam ...args_t are the arguments types to forward to the component's constructor.
        /// @exception Throws an entt exception if the specified entity is not contained by the
        /// registry.
        template <typename component_t, typename... args_t>
        decltype(auto) create_component(const entity_t entity, args_t&&... args);

        /// @brief Gets an existing component from the specified entity.
        /// @tparam component_t is the component type to get.
        /// @exception Throws an entt exception if the specified entity is not contained by the
        /// registry, or if this entity doesn't have a component of this type.
        template <typename component_t>
        component_t& get_component(const entity_t entity);

        /// @brief Gets an existing const component from the specified entity.
        /// @tparam component_t is the component type to get.
        /// @exception Throws an entt exception if the specified entity is not contained by the
        /// registry, or if this entity doesn't have a component of this type.
        /// @tparam component_t is the component type to get.
        template <typename component_t>
        const component_t& get_component(const entity_t entity) const;

        /// @brief Iterate over components in the registry.
        /// @tparam ...components_t are the component types to select for iteration.
        template <typename... components_t> // exclude filter with strong_typelist
        void iterate(const std::function<void(components_t&...)>& iterate_function);

        /// @brief Iterate over components and entities in the registry.
        /// @tparam ...components_t are the component types to select for iteration.
        template <typename... components_t> // exclude filter with strong_typelist
        void iterate_with_entities(const std::function<void(entity_t, components_t&...)>& iterate_function);

        std::size_t size() const;

    private:
        entt::basic_registry<entity_t> _registry;
    };
}
}

#include <bungeegum/glue/registry.inl>