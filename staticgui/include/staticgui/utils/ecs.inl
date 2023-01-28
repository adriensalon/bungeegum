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

        // iterate_entities

        template <typename... components_t>
        void registry::iterate_components(std::function<void(components_t&...)> iterate_function)
        {
            auto& _view = _registry.view<components_t...>();
            _view.each([&](components_t&... _components) {
                iterate_function(_components...);
            });
        }

        template <typename... components_t, typename function_t>
        void registry::iterate_entities_components(function_t&& iterate_function)
        {
            // static_assert(std::is_invocable_v<function_t, const components_t&...>);
            auto& _view = _registry->view<components_t...>();
            _view.each([&](entt::entity _raw_ent, components_t&... _components) {
                entity _ent(*this, _raw_ent);
                iterate_function(_ent, _components...);
            });
        }

        template <typename component_t>
        bool entity::has_component() const
        {
        }

        template <typename component_t, typename... args_t>
        component_t& entity::create_component(args_t&&... args)
        {
            return _registry->emplace<component_t>(_entity, std::forward<args_t>(args)...);
        }

        template <typename component_t>
        component_t& entity::get_component()
        {
        }

        template <typename component_t>
        const component_t& entity::get_component() const
        {
        }

        template <typename component_t>
        component_t& entity::get_or_create_component()
        {
        }

        template <typename component_t>
        const component_t& entity::get_or_create_component() const
        {
        }
    }
}
}