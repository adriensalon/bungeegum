//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/utils/ecs.hpp>

namespace staticgui {
namespace internal {
    namespace ecs {

        registry::registry()
        {
            _registry = std::make_shared<entt::registry>();
        }

        registry::registry(const registry& other)
        {
            *this = other;
        }

        registry& registry::operator=(const registry& other)
        {
            _registry = other._registry;
            return *this;
        }

        registry::registry(registry&& other)
        {
            *this = std::move(other);
        }

        registry& registry::operator=(registry&& other)
        {
            _registry = std::move(other._registry);
            return *this;
        }

        entity::entity(registry& reg)
        {
            _entity = reg._registry->create();
            _registry = reg._registry;
        }

        entity::entity(registry& reg, const entt::entity raw_ent)
        {
            _entity = raw_ent;
            _registry = reg._registry;
        }

        entity::entity(const entity& other)
        {
            *this = other;
        }

        entity& entity::operator=(const entity& other)
        {
            _entity = other._entity;
            _registry = other._registry;
            return *this;
        }

        entity::entity(entity&& other)
        {
            *this = std::move(other);
        }

        entity& entity::operator=(entity&& other)
        {
            _entity = std::move(other._entity); // no effect but for consistency ?
            _registry = std::move(other._registry); // captures the std::shared_ptr
            return *this;
        }
    }
}
}