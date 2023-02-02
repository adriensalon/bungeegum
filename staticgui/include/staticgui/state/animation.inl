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

    template <typename value_t>
    animation_impl<value_t>& animation_registry::make_animation_and_data()
    {
        glue::id_integer _entity = _registry.create_entity();
        animation_data& _animation_data = _registry.create_component<animation_data>(_entity);
        _animation_data.tick = [&](const float delta_milliseconds) {
            // todo
        };
        _animation_data.kind = std::make_unique<std::type_index>(typeid(value_t));
        return _registry.create_component<animation_impl<value_t>>(_entity);
    }

    template <typename value_t>
    animation_data& animation_registry::get_data(animation_impl<value_t>& animation)
    {
        glue::id_integer _entity = _registry.get_entity(animation);
        return _registry.get_component<animation_data>(_entity);
    }

    template <typename value_t>
    animation_impl<value_t>& animation_registry::get_animation(animation_data& data)
    {
        glue::id_integer _entity = _registry.get_entity(data);
        return _registry.get_component<animation_impl<value_t>>(_entity);
    }

}
}