//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <variant>

namespace staticgui {
namespace glue {

    // template <typename component_t, typename... args_t>
    // component_t& registry::bool has_component(const entity entity) const
    // {
    // }

    template <typename component_t>
    component_t& registry::get_component(const entity entity)
    {
        // return _registry.view<component_t, float>().get<component_t>(entity);
        return _registry.get<component_t>(entity);
        // return vv;
    }

    template <typename component_t>
    const component_t& registry::get_component(const entity entity) const
    {
        return _registry.view<component_t>().get<component_t>(entity);
    }

    template <typename component_t, typename... args_t>
    decltype(auto) registry::create_component(const entity entity, args_t&&... args)
    {
        return _registry.emplace<component_t>(entity, std::forward<args_t>(args)...);
    }

    template <typename... components_t>
    void registry::iterate_components(const std::function<void(components_t&...)>& iterate_function)
    {
        auto& _view = _registry.view<components_t...>();
        _view.each([&](components_t&... _components) {
            iterate_function(_components...);
        });
    }

    template <typename... components_t>
    void registry::iterate_entities_components(const std::function<void(entity, components_t&...)>& iterate_function)
    {
        // static_assert(std::is_invocable_v<function_t, const components_t&...>);
        auto& _view = _registry.view<components_t...>();
        _view.each([&](entity _entity, components_t&... _components) {
            iterate_function(_entity, _components...);
        });
    }

}
}