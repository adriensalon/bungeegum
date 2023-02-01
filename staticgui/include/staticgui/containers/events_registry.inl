//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {
namespace detail {

    template <typename... values_t>
    event_impl<values_t...>& events_registry::make_event_and_data()
    {
        glue::id_integer _entity = _registry.create_entity();
        event_data& _event_data = _registry.create_component<event_data>(_entity);
        _event_data.tick = [&]() {
            // todo
        };
        (_event_data.kinds.emplace_back(typeid(values_t)), ...);
        _roots.emplace_back(_event_data);
        return _registry.create_component<event_impl<values_t...>>(_entity);
    }
}
}