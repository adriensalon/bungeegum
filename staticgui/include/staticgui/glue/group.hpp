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

#include <staticgui/glue/id_integer.hpp>
#include <staticgui/glue/typelist.hpp>

namespace staticgui {
namespace glue {

    // exclude filter
    template <typename... values_t> // https://skypjack.github.io/2019-04-12-entt-tips-and-tricks-part-1/
    struct group {
        group();
        group(const group& other);
        group& operator=(const group& other);
        group(group&& other);
        group& operator=(group&& other);

        template <typename component_t, typename... args_t>
        component_t& create_component(const id_integer entity, args_t&&... args);

        template <typename... components_t>
        void iterate_components(const std::function<void(components_t&...)>& iterate_function);

        template <typename... components_t>
        void iterate_entities_components(const std::function<void(const id_integer, components_t&...)>& iterate_function);

    private:
        entt::basic_group<id_integer, values_t...> _group;
    };

}
}

#include <staticgui/glue/group.inl>