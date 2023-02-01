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
#include <optional>
#include <typeindex>
#include <vector>

#include <staticgui/glue/registry.hpp>

namespace staticgui {
namespace detail {

    struct event_disabled {
    };

    template <typename values_t>
    struct event_impl {
    };

    struct event_data {
        std::function<void()> tick = nullptr;
        std::vector<std::type_index> kind;
        std::optional<std::reference_wrapper<event_data>> parent = std::nullopt;
        std::vector<std::reference_wrapper<event_data>> children = {};
    };

    struct events_registry {

    private:
        glue::registry _registry;
    };

}
}

#include <staticgui/containers/events_registry.inl>
