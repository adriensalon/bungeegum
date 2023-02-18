//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <bungeegum/context/exceptions.hpp>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    typed_animation_data<value_t>& animations_registry::create(events_registry& events)
    {
        std::cout << "create anim \n";
        entity_t _entity = animations.create_entity();
        typed_animation_data<value_t>& _animation = animations.create_component<typed_animation_data<value_t>>(_entity);
        untyped_animation_data& _animation_data = animations.create_component<untyped_animation_data>(_entity);
        _animation_data.tick = [this, &_animation, &_animation_data](const std::chrono::milliseconds& delta_time) {
            if (_animation_data.is_playing) {
                _animation.playing_cursor_seconds += 0.001f * delta_time.count();
                if (_animation.playing_cursor_seconds > _animation.duration_seconds) {
                    std::cout << "STOPPED \n";
                    _animation.playing_cursor_seconds = _animation.duration_seconds;
                    stop(_animation);
                }
                float _frac = _animation.playing_cursor_seconds / _animation.duration_seconds;
                _animation_data.eval_point = _animation_data.eval_curve.eval(_frac);
                float _t = _animation_data.eval_point.y;
                value_t _lerped = lerp<value_t>(std::forward<value_t>(*(_animation.min_value)), std::forward<value_t>(*(_animation.max_value)), _t);
                trigger<value_t>(_animation.event, std::forward<value_t>(_lerped));
            }
        };
        _animation_data.kind = std::make_unique<std::type_index>(typeid(value_t));
        return _animation;
    }

    template <typename value_t>
    void animations_registry::destroy(const typed_animation_data<value_t>& animation)
    {
    }

    template <typename value_t>
    void animations_registry::start(typed_animation_data<value_t>& animation)
    {
        get(animation).is_playing = true;
    }

    template <typename value_t>
    void animations_registry::stop(typed_animation_data<value_t>& animation)
    {
        get(animation).is_playing = false;
    }

    template <typename value_t>
    void animations_registry::reset(typed_animation_data<value_t>& animation)
    {
        animation.playing_cursor_seconds = 0.f;
    }

    template <typename value_t>
    void animations_registry::shape(typed_animation_data<value_t>& animation, const curve& animation_curve)
    {
        get(animation).eval_curve = animation_curve;
    }

    template <typename value_t>
    void animations_registry::set_animation_min(typed_animation_data<value_t>& animation, value_t&& min_value)
    {
        animation.min_value = std::make_unique<value_t>(std::forward<value_t>(min_value));
        if (animation.min_value && animation.max_value)
            if ((*animation.max_value) < (*animation.min_value))
                throw_library_bad_usage("Bad animation min value");
    }

    template <typename value_t>
    void animations_registry::set_animation_max(typed_animation_data<value_t>& animation, value_t&& max_value)
    {
        animation.max_value = std::make_unique<value_t>(std::forward<value_t>(max_value));
        if (animation.min_value && animation.max_value)
            if ((*animation.max_value) < (*animation.min_value))
                throw_library_bad_usage("Bad animation max value");
    }

    template <typename value_t, typename duration_unit_t>
    void animations_registry::set_animation_duration(typed_animation_data<value_t>& animation, const unsigned int count)
    {
        animation.duration_seconds = 0.001f * std::chrono::duration_cast<std::chrono::milliseconds>(duration_unit_t(count)).count();
    }

    template <typename value_t>
    void animations_registry::attach_animation(typed_animation_data<value_t>& animation)
    {
    }

    template <typename value_t>
    void animations_registry::detach_animation(typed_animation_data<value_t>& animation)
    {
    }

    template <typename value_t>
    untyped_animation_data& animations_registry::get(typed_animation_data<value_t>& animation)
    {
        entity_t _entity = animations.get_entity(animation);
        return animations.get_component<untyped_animation_data>(_entity);
    }

    template <typename value_t>
    typed_animation_data<value_t>& animations_registry::get(untyped_animation_data& data)
    {
        entity_t _entity = animations.get_entity(data);
        return animations.get_component<typed_animation_data<value_t>>(_entity);
    }

}

template <typename value_t>
template <typename duration_unit_t>
animation<value_t>::animation(
    const curve& curved_shape,
    lerpable_value&& min_value,
    lerpable_value&& max_value,
    const unsigned int duration_count,
    const animation_mode mode)
    : _data(detail::animations_context.create<value_t>(detail::events_context))
{
    detail::animations_context.shape(_data, curved_shape);
    detail::animations_context.set_animation_min(_data, std::forward<value_t>(min_value));
    detail::animations_context.set_animation_max(_data, std::forward<value_t>(max_value));
    detail::animations_context.set_animation_duration<value_t, duration_unit_t>(_data, duration_count);
    // mode
}

template <typename value_t>
animation<value_t>::animation(const animation<value_t>& other)
{
    *this = other;
}

template <typename value_t>
animation<value_t>& animation<value_t>::operator=(const animation<value_t>& other)
{
    _data = other._data;
    return *this;
}

template <typename value_t>
animation<value_t>::animation(animation<value_t>&& other)
{
    *this = std::move(other);
}

template <typename value_t>
animation<value_t>& animation<value_t>::operator=(animation<value_t>&& other)
{
    _data = std::move(other._data);
    return *this;
}

template <typename value_t>
animation<value_t>::~animation()
{
    if (_data.event.is_attached)
        detail::animations_context.destroy(_data);
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    detail::merge(_data.event, value_changed_event);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_tick(const on_tick_callback& tick_callback)
{
    _data.event.callbacks.push_back(tick_callback);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::start()
{
    detail::animations_context.start(_data);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::stop()
{
    detail::animations_context.stop(_data);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::reset()
{
    detail::animations_context.reset(_data);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::shape(const curve& curved_shape)
{
    detail::animations_context.shape(_data, curved_shape);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::min(value_t&& min_value)
{
    detail::animations_context.set_animation_min(_data, std::forward<value_t>(min_value));
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::max(value_t&& max_value)
{
    detail::animations_context.set_animation_max(_data, std::forward<value_t>(max_value));
    return *this;
}

template <typename value_t>
template <typename duration_unit_t>
animation<value_t>& animation<value_t>::duration(const unsigned int count)
{
    detail::animations_context.set_animation_duration<value_t, duration_unit_t>(_data, count);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::attach()
{
    detail::animations_context.attach_animation(_data);
    return *this;
}

template <typename value_t>
template <typename widget_t>
animation<value_t>& animation<value_t>::detach(widget_t& widget)
{
    detail::animations_context.detach_animation(_data, widget);
}

template <typename value_t>
animation<value_t>& animation<value_t>::detach()
{
    detail::animations_context.detach_animation(_data);
    return *this;
}

}