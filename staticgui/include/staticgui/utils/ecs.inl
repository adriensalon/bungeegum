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

        template <typename value_t>
        registry<value_t>::registry()
        {
        }

        template <typename value_t>
        registry<value_t>::registry(registry&& other)
        {
        }

        template <typename value_t>
        registry<value_t>& registry<value_t>::operator=(registry&& other)
        {
            return *this;
        }

        template <typename value_t>
        entity<value_t>& registry<value_t>::create_entity()
        {
        }

        template <typename value_t>
        template <typename... tags_t, typename... exclude_tags_t, typename... components_t>
        void registry<value_t>::iterate_components(tags_filter<tags_t...> tags, tags_exclude_filter<exclude_tags_t...> exclude_tags, std::function<void(components_t&...)> iterate_function)
        {
        }

        template <typename value_t>
        template <typename... tags_t, typename... exclude_tags_t, typename... components_t>
        void registry<value_t>::iterate_entities_and_components(tags_filter<tags_t...> tags, tags_exclude_filter<exclude_tags_t...> exclude_tags, std::function<void(entity<value_t>&, components_t&...)> iterate_function)
        {
        }

        template <typename value_t>
        entity<value_t>::entity(const registry<value_t>& reg, const value_t& value)
        {
        }

        template <typename value_t>
        entity<value_t>::entity(entity&& other)
        {
        }

        template <typename value_t>
        entity<value_t>& entity<value_t>::operator=(entity&& other)
        {
            return *this;
        }

        template <typename value_t>
        value_t& entity<value_t>::get_value()
        {
        }

        template <typename value_t>
        registry<value_t>& entity<value_t>::get_registry() const
        {
        }

        template <typename value_t>
        template <typename component_t>
        bool entity<value_t>::has_component() const
        {
        }

        template <typename value_t>
        template <typename component_t>
        component_t& entity<value_t>::create_component()
        {
        }

        template <typename value_t>
        template <typename component_t>
        component_t& entity<value_t>::get_component()
        {
        }

        template <typename value_t>
        template <typename component_t>
        const component_t& entity<value_t>::get_component() const
        {
        }

        template <typename value_t>
        template <typename component_t>
        component_t& entity<value_t>::get_or_create_component()
        {
        }

        template <typename value_t>
        template <typename component_t>
        const component_t& entity<value_t>::get_or_create_component() const
        {
        }
    }
}
}