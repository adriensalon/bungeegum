#pragma once

#include <bungeegum/glue/lerp.hpp>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    animation_data<value_t>::~animation_data()
    {
        if (global().animations.contains(raw_animation)) {
            global().animations.notify_erase(raw_animation);
        }
    }

    template <typename value_t>
    void assign_ticker(animation_data<value_t>& data, animation_update_data& update_data)
    {
        update_data.ticker = [&](const std::chrono::milliseconds& delta_time) {
            if (!data.is_playing) {
                global().animations.notify_erase(data.raw_animation);
                return;
            }
            if (data.playing_cursor_seconds > data.duration_seconds) {
                data.playing_cursor_seconds = data.duration_seconds;
                data.is_playing = false;
                global().animations.notify_erase(data.raw_animation);
                return;
            }
            float _frac = data.playing_cursor_seconds / data.duration_seconds;
            float2 _curve_eval = data.eval_curve.evaluate(_frac);
#if BUNGEEGUM_USE_OVERLAY
            update_data.overlay_position = { _curve_eval.x, _curve_eval.y };
            update_data.overlay_samples = data.eval_curve.strided_samples(100);
#endif
            float _t = _curve_eval.y;
            value_t _lerped = lerp<value_t>(std::forward<value_t>(*(data.min_value)), std::forward<value_t>(*(data.max_value)), _t);
            for (auto& _callback : data.event.callbacks)
                _callback(std::forward<value_t>(_lerped));
            data.playing_cursor_seconds += 0.001f * delta_time.count();
        };
    }
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    _data.event.merge(value_changed_event);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const on_value_changed_callback& tick_callback)
{
    _data.event.callbacks.push_back(tick_callback);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::start()
{
    std::uintptr_t _raw_animation = detail::raw_cast<animation<value_t>>(this);
    if (!detail::global().animations.contains(_raw_animation)) {
        detail::animation_update_data& _update_data = detail::global().animations[_raw_animation];
        _data.raw_animation = _raw_animation;
        _update_data.kind = std::make_unique<std::type_index>(typeid(value_t));
        detail::assign_ticker(_data, _update_data);
#if BUNGEEGUM_USE_OVERLAY
        _update_data.clean_typename = detail::backend_manager::to_clean_typename(_update_data.kind->name());
#endif
    }
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
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::max(value_t&& max_value)
{
    _data.max_value = std::make_unique<value_t>(std::forward<value_t>(max_value));
    return *this;
}

template <typename value_t>
template <typename duration_unit_t>
animation<value_t>& animation<value_t>::duration(const unsigned int count)
{
    _data.duration_seconds = 0.001f * std::chrono::duration_cast<std::chrono::milliseconds>(duration_unit_t(count)).count();
    return *this;
}
}