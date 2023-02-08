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
    animation_impl<value_t>& animation_registry::make_animation_and_data(event_registry& events)
    {
        glue::entity _entity = _registry.create_entity();
        animation_impl<value_t>& _animation = _registry.create_component<animation_impl<value_t>>(_entity);
        animation_data& _animation_data = _registry.create_component<animation_data>(_entity);
        _animation_data.tick = [&](const float delta_milliseconds) {
            if (_animation.is_playing) {
                _animation.playing_cursor_seconds += 0.001f * delta_milliseconds;
                if (_animation.playing_cursor_seconds > _animation.duration_seconds) {
                    std::cout << "STOPPED \n";
                    _animation.playing_cursor_seconds = _animation.duration_seconds;
                    stop_animation(_animation);
                }
                float _frac = _animation.playing_cursor_seconds / _animation.duration_seconds;

                float _t = _animation.curve.spline->get_eval(_frac).y();
                value_t _lerped = lerp<value_t>(std::forward<value_t>(*(_animation.min_value)), std::forward<value_t>(*(_animation.max_value)), _t);
                events.trigger_values<value_t>(_animation.event, std::forward<value_t>(_lerped));
            }
        };
        _animation_data.kind = std::make_unique<std::type_index>(typeid(value_t));
        return _animation;
    }

    template <typename value_t>
    void animation_registry::destroy_animation_and_data(const animation_impl<value_t>& animation)
    {
    }

    template <typename value_t>
    void animation_registry::start_animation(animation_impl<value_t>& animation)
    {
        animation.is_playing = true;
    }

    template <typename value_t>
    void animation_registry::stop_animation(animation_impl<value_t>& animation)
    {
        animation.is_playing = false;
    }

    template <typename value_t>
    void animation_registry::reset_animation(animation_impl<value_t>& animation)
    {
        animation.playing_cursor_seconds = 0.f;
    }

    template <typename value_t>
    void animation_registry::shape_animation(animation_impl<value_t>& animation, const curve_data& curve)
    {
        animation.curve.spline = curve.spline;
    }

    template <typename value_t>
    void animation_registry::set_animation_min(animation_impl<value_t>& animation, value_t&& min_value)
    {
        animation.min_value = std::make_unique<value_t>(std::forward<value_t>(min_value));
        if (animation.min_value && animation.max_value)
            if ((*animation.max_value) < (*animation.min_value))
                throw_library_bad_usage("Bad animation min value");
    }

    template <typename value_t>
    void animation_registry::set_animation_max(animation_impl<value_t>& animation, value_t&& max_value)
    {
        animation.max_value = std::make_unique<value_t>(std::forward<value_t>(max_value));
        if (animation.min_value && animation.max_value)
            if ((*animation.max_value) < (*animation.min_value))
                throw_library_bad_usage("Bad animation max value");
    }

    template <typename value_t, typename duration_unit_t>
    void animation_registry::set_animation_duration(animation_impl<value_t>& animation, const unsigned int count)
    {
        animation.duration_seconds = 0.001f * std::chrono::duration_cast<std::chrono::milliseconds>(duration_unit_t(count)).count();
    }

    template <typename value_t>
    void animation_registry::attach_animation(animation_impl<value_t>& animation)
    {
    }

    template <typename value_t>
    void animation_registry::detach_animation(animation_impl<value_t>& animation)
    {
    }

    template <typename value_t>
    animation_data& animation_registry::get_data(animation_impl<value_t>& animation)
    {
        glue::entity _entity = _registry.get_entity(animation);
        return _registry.get_component<animation_data>(_entity);
    }

    template <typename value_t>
    animation_impl<value_t>& animation_registry::get_animation(animation_data& data)
    {
        glue::entity _entity = _registry.get_entity(data);
        return _registry.get_component<animation_impl<value_t>>(_entity);
    }

}
}