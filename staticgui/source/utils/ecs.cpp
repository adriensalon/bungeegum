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

        registry::registry() { }

        registry::registry(registry&& other)
        {
            *this = std::move(other);
        }

        registry& registry::operator=(registry&& other)
        {
            _registry = std::move(other._registry);
            return *this;
        }

        entity::entity()
        {
            _is_valid = false;
            _registry_ptr = nullptr;
        }

        bool entity::is_valid() const
        {
            return _is_valid;
        }

        entity::entity(registry& reg)
        {
            _is_valid = true;
            _entity = reg._registry.create();
            _registry_ptr = &(reg._registry);
        }

        entity::entity(registry& reg, const entt::entity raw_ent)
        {
            _is_valid = true;
            _entity = raw_ent;
            _registry_ptr = &(reg._registry);
        }

        entity::entity(const entity& other)
        {
            *this = other;
        }

        entity& entity::operator=(const entity& other)
        {
            _is_valid = other._is_valid;
            _entity = other._entity;
            _registry_ptr = other._registry_ptr;
            return *this;
        }
    }
}
}