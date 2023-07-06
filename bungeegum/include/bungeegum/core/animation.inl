#pragma once

#include <bungeegum/core/exceptions.hpp>

namespace bungeegum {
namespace detail {

    // typed_animation_data

    template <typename value_t>
    typed_animation_data<value_t>::~typed_animation_data()
    {
        if (is_animation_possessed(raw_animation))
            animations_context.possessed.erase(raw_animation);
        if (is_animation_registered(raw_animation))
            animations_context.registered.erase(raw_animation);
    }

    // animations_registry

    template <typename value_t>
    typed_animation_data<value_t>& animations_registry::get_typed(animation<value_t>& animation_object)
    {
        return animation_object._data;
    }

    // free

    enum animation_raw_access_mode {
        animation_recast,
        animation_stored
    };

    template <animation_raw_access_mode access_mode_t, typename value_t>
    std::uintptr_t get_raw_animation(animation<value_t>& animation_object)
    {
        if constexpr (access_mode_t == animation_raw_access_mode::animation_recast)
            return raw_cast(animation_object);
        else if constexpr (access_mode_t == animation_raw_access_mode::animation_stored)
            return animations_context.template get_typed<value_t>(animation_object).raw_animation;
    }

    inline bool is_animation_registered(const std::uintptr_t raw_animation)
    {
        return animations_context.registered.find(raw_animation) != animations_context.registered.end();
    }

    template <animation_raw_access_mode access_mode_t, typename value_t>
    bool is_animation_registered(animation<value_t>& animation_object)
    {
        std::uintptr_t _raw_animation = template get_raw_animation<access_mode_t, value_t>(animation_object);
        return is_animation_registered(_raw_animation);
    }

    inline bool is_animation_possessed(const std::uintptr_t raw_animation)
    {
        return animations_context.possessed.find(raw_animation) != animations_context.possessed.end();
    }

    template <animation_raw_access_mode access_mode_t, typename value_t>
    bool is_animation_possessed(animation<value_t>& animation_object)
    {
        std::uintptr_t _raw_animation = template get_raw_animation<access_mode_t, value_t>(animation_object);
        return is_animation_possessed(_raw_animation);
    }

    template <typename value_t>
    void assign_on_tick(typed_animation_data<value_t>& typed_animation, untyped_animation_data& untyped_animation)
    {
        untyped_animation.ticker = [&](const std::chrono::milliseconds& delta_time) {
            if (typed_animation.is_playing) {
                typed_animation.playing_cursor_seconds += 0.001f * delta_time.count();
                if (typed_animation.playing_cursor_seconds > typed_animation.duration_seconds) {
                    std::cout << "STOPPED animation \n";
                    typed_animation.playing_cursor_seconds = typed_animation.duration_seconds;
                    typed_animation.is_playing = false;
                }
                float _frac = typed_animation.playing_cursor_seconds / typed_animation.duration_seconds;
                float2 _curve_eval = typed_animation.eval_curve.evaluate(_frac);
                // ifdef protected
                untyped_animation.overlay_position = { _curve_eval.x, _curve_eval.y };
                untyped_animation.overlay_samples = typed_animation.eval_curve.strided_samples(100);
                //
                float _t = _curve_eval.y;
                value_t _lerped = lerp<value_t>(std::forward<value_t>(*(typed_animation.min_value)), std::forward<value_t>(*(typed_animation.max_value)), _t);
                for (auto& _callback : typed_animation.event.callbacks)
                    _callback(std::forward<value_t>(_lerped));
            }
        };
    }

    template <typename value_t>
    void assign_animation(animation<value_t>& animation_object, untyped_animation_data& untyped_animation, std::uintptr_t raw_animation)
    {
        typed_animation_data<value_t>& _typed_animation = animations_context.template get_typed<value_t>(animation_object);
        _typed_animation.raw_animation = raw_animation;
        untyped_animation.kind = std::make_unique<std::type_index>(typeid(value_t));
        animations_context.registered.insert_or_assign(raw_animation, std::ref(untyped_animation));
        assign_on_tick(_typed_animation, untyped_animation);
    }

    template <typename value_t>
    void register_animation(animation<value_t>& animation_object, const std::uintptr_t raw_animation)
    {
        registry_entity _entity;
        if (is_animation_possessed(raw_animation)) {
            _entity = animations_context.possessed.at(raw_animation);
            untyped_animation_data& _untyped_animation = animations_context.animations.get_component<untyped_animation_data>(_entity);
            assign_animation(animation_object, _untyped_animation, raw_animation);
            return;
        } else {
            std::optional<registry_entity> _existing_entity = animations_context.animations.try_get_entity(animation_object);
            if (_existing_entity != std::nullopt) {
                _entity = _existing_entity.value();
                untyped_animation_data& _untyped_animation = animations_context.animations.get_component<untyped_animation_data>(_entity);
                assign_animation(animation_object, _untyped_animation, raw_animation);
                return;
            }
        }
        _entity = animations_context.animations.create_entity();
        animations_context.animations.template create_component<std::reference_wrapper<animation<value_t>>>(_entity, animation_object);
        untyped_animation_data& _untyped_animation = animations_context.animations.template create_component<untyped_animation_data>(_entity);
        assign_animation(animation_object, _untyped_animation, raw_animation);
    }
}

// animation

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    _data.event.merge(value_changed_event);
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
    std::uintptr_t _raw_animation = detail::get_raw_animation<detail::animation_raw_access_mode::animation_recast>(*this);
    if (!detail::is_animation_registered(_raw_animation))
        detail::register_animation(*this, _raw_animation);
    _data.is_playing = true;
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::stop()
{
    _data.is_playing = false;
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::reset()
{
    _data.playing_cursor_seconds = 0.f;
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::shape(const curve& curved_shape)
{
    _data.eval_curve = curved_shape;
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::min(value_t&& min_value)
{
    _data.min_value = std::make_unique<value_t>(std::forward<value_t>(min_value));
    if (_data.min_value && _data.max_value)
        if (*(_data.max_value) < *(_data.min_value))
            detail::throw_error<detail::error_type::bad_usage>("Bad animation min value");
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::max(value_t&& max_value)
{
    _data.max_value = std::make_unique<value_t>(std::forward<value_t>(max_value));
    if (_data.min_value && _data.max_value)
        if (*(_data.max_value) < *(_data.min_value))
            detail::throw_error<detail::error_type::bad_usage>("Bad animation max value");
    return *this;
}

template <typename value_t>
template <typename duration_unit_t>
animation<value_t>& animation<value_t>::duration(const unsigned int count)
{
    _data.duration_seconds = 0.001f * std::chrono::duration_cast<std::chrono::milliseconds>(duration_unit_t(count)).count();
    return *this;
}

// free

template <typename value_t>
animation<value_t>& make_animation()
{
    detail::registry_entity _entity = detail::animations_context.animations.create_entity();
    detail::animations_context.animations.create_component<detail::untyped_animation_data>(_entity);
    animation<value_t>& _animation = detail::animations_context.animations.template create_component<animation<value_t>>(_entity);
    std::uintptr_t _raw_animation = detail::get_raw_animation<detail::animation_raw_access_mode::animation_recast>(_animation);
    detail::animations_context.possessed.emplace(_raw_animation, _entity);
    return _animation;
}

template <typename value_t>
animation<value_t>& make_animation(const animation<value_t>& other_animation)
{
    detail::registry_entity _entity = detail::animations_context.animations.create_entity();
    detail::animations_context.animations.create_component<detail::untyped_animation_data>(_entity);
    animation<value_t>& _animation = detail::animations_context.animations.template create_component<animation<value_t>>(_entity, other_animation);
    std::uintptr_t _raw_animation = detail::get_raw_animation<detail::animation_raw_access_mode::animation_recast>(_animation);
    detail::animations_context.possessed.emplace(_raw_animation, _entity);
    return _animation;
}

template <typename value_t>
animation<value_t>& make_animation(animation<value_t>&& other_animation)
{
    detail::registry_entity _entity = detail::animations_context.animations.create_entity();
    detail::animations_context.animations.create_component<detail::untyped_animation_data>(_entity);
    animation<value_t>& _animation = detail::animations_context.animations.create_component<animation<value_t>>(_entity, std::move(other_animation));
    std::uintptr_t _raw_animation = detail::get_raw_animation<detail::animation_raw_access_mode::animation_recast>(_animation);
    detail::animations_context.possessed.emplace(_raw_animation, _entity);
    return _animation;
}

template <typename value_t>
void unmake_animation(animation<value_t>& made_animation)
{
    std::uintptr_t _raw_animation = detail::get_raw_animation<detail::animation_raw_access_mode::animation_stored>(made_animation);
    detail::registry_entity _entity = detail::animations_context.possessed.at(_raw_animation);
    detail::animations_context.animations.destroy_entity(_entity);
}
}