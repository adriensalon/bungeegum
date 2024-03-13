#pragma once

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/event.hpp>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    void assign_ticker(animation_data<value_t>& data)
    {
        data.update_data.ticker = [&data](const std::chrono::milliseconds& delta_time, animations_manager_data& animations) {

            if (data.playing_cursor_seconds > data.duration_seconds) {
                // switch mode
                data.playing_cursor_seconds = data.duration_seconds;
                data.is_playing = false;
            }

            if (!data.is_playing) {
                for (std::function<void()>& _callback : data.on_end_event.callbacks) {
                    _callback();
                }
                animations.updatables_to_erase.push_back(data.raw);
                return;
            }

            float _frac = data.playing_cursor_seconds / data.duration_seconds;
            float2 _curve_eval = { _frac, data.eval_curve.evaluate_1d(_frac) };
#if BUNGEEGUM_USE_OVERLAY
            data.update_data.overlay_position = { _curve_eval.x, _curve_eval.y };
            data.update_data.overlay_samples = data.eval_curve.get_strided_samples(100);
#endif
            float _t = _curve_eval.y;
            value_t _lerped = math::lerp<value_t>(std::forward<value_t>(*(data.min_value)), std::forward<value_t>(*(data.max_value)), _t);
            for (auto& _callback : data.on_value_changed_event.callbacks) {
                _callback(std::forward<value_t>(_lerped));
            }
            data.playing_cursor_seconds += 0.001f * delta_time.count();
        };
    }
    
    template <typename value_t>
    animation_data<value_t>::animation_data()
    {        
        raw = raw_cast(this); // create a new id
        (update_data.kinds.push_back(typeid(values_t)), ...);
    }

    template <typename value_t>
    animation_data<value_t>::animation_data(const animation_data& other)
    {
        *this = other;
    }

    template <typename value_t>
    animation_data<value_t>& animation_data<value_t>::operator=(const animation_data<value_t>& other)
    {
        raw = raw_cast(this); // create a new id
        update_data.kind = other.update_data.kind;
        is_playing = false; // reset

        min_value = other.min_value;
        max_value = other.max_value;
        on_value_changed_event = other.on_value_changed_event; // events are reset
        on_end_event = other.on_end_event; // events are reset
        bspline = other.bspline;
        duration_seconds = other.duration_seconds;
        playing_cursor_seconds = other.playing_cursor_seconds;
        mode = other.mode;
        return *this;
    }

    template <typename value_t>
    animation_data<value_t>::animation_data(animation_data&& other)
    {
        *this = std::move(other);
    }

    template <typename value_t>
    animation_data<value_t>& animation_data<value_t>::operator=(animation_data<value_t>&& other)
    {
        raw = other.raw; // keep the same id
        update_data.kind = other.update_data.kind;

        is_playing = std::move(other.is_playing);
        min_value = std::move(other.min_value);
        max_value = std::move(other.max_value);
        on_value_changed_event = std::move(other.on_value_changed_event);
        on_end_event = std::move(other.on_end_event);
        bspline = std::move(other.bspline);
        duration_seconds = std::move(other.duration_seconds);
        playing_cursor_seconds = std::move(other.playing_cursor_seconds);
        mode = std::move(other.mode);

        swapped_manager_data& _swapped = get_swapped_global();
        if (_swapped.animations.updatables.find(raw) != _swapped.animations.updatables.end()) {
            _swapped.animations.updatables.at(raw) = std::ref(update_data); // update ref
            assign_ticker(*this);
        }
        return *this;
    }

    template <typename value_t>
    animation_data<value_t>::~animation_data()
    {
        swapped_manager_data& _swapped = get_swapped_global();
        if (_swapped.animations.updatables.find(raw) != _swapped.animations.updatables.end()) {
            _swapped.animations.updatables_to_erase.push_back(raw);
        }
    }
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_end(const event<>& end_event)
{
    _data.on_end_event.merge(end_event);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_end(const on_end_callback& end_callback)
{
    _data.on_end_event.callbacks.push_back(end_callback);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    _data.on_value_changed_event.merge(value_changed_event);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const on_value_changed_callback& value_changed_callback)
{
    _data.on_value_changed_event.callbacks.push_back(value_changed_callback);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::start()
{
    std::uintptr_t _raw_animation = detail::raw_cast<animation<value_t>>(this);
    if (!detail::get_swapped_global().animations.contains(_raw_animation)) {
        detail::animation_update_data& _update_data = detail::get_swapped_global().animations[_raw_animation];
        _data.raw_animation = _raw_animation;
        _update_data.kind = std::make_unique<std::type_index>(typeid(value_t));
        detail::assign_ticker(_data, _update_data);
#if BUNGEEGUM_USE_OVERLAY
        // _update_data.clean_typename = detail::pipeline_manager_data::to_clean_typename(_update_data.kind->name());
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
animation<value_t>& animation<value_t>::shape(const curve& curved_shape)
{
    _data.eval_curve = curved_shape;
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::min(const value_t& min_value)
{
    _data.min_value = min_value;
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::max(const value_t& max_value)
{
    _data.max_value = max_value;
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::mode(const animation_mode mode)
{
    _data.mode = mode;
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