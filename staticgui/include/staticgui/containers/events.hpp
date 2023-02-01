//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>
#include <typeindex>
#include <vector>

#include <staticgui/glue/id_integer.hpp>
#include <staticgui/glue/registry.hpp>

namespace staticgui {
namespace detail {

    struct event_component {
        std::vector<glue::any_function> callbacks;
        std::vector<std::type_index> typeinfos;
        glue::id_integer entity;
    };

    struct events_registry {

    private:
        glue::registry _registry;
    };

}
}