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

template <typename... values_t>
event(std::function<void(values_t...)>) -> event<values_t...>;

namespace detail {
    inline static host_state state;

    template <typename widget_t, typename = std::void_t<>>
    constexpr bool has_resolve = false;
    template <typename widget_t>
    constexpr bool has_resolve<widget_t, std::void_t<decltype(std::declval<widget_t>().resolve(std::declval<const resolve_constraint&>(), std::declval<resolve_advice&>()))>> = true;

    template <typename widget_t, typename = std::void_t<>>
    constexpr bool has_draw = false;
    template <typename widget_t>
    constexpr bool has_draw<widget_t, std::void_t<decltype(std::declval<widget_t>().draw(std::declval<draw_command&>()))>> = true;
}

#pragma region event
template <typename... values_t>
event<values_t...>::event()
    : _impl(detail::state.context.events.make_event_and_data<values_t...>())
{
}

template <typename... values_t>
event<values_t...>::event(const on_trigger_callback& trigger_callback)
    : _impl(detail::state.context.events.make_event_and_data<values_t...>())
{
    _impl.callbacks.push_back(trigger_callback);
}

template <typename... values_t>
event<values_t...>::event(const event& other)
{
    *this = other;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::operator=(const event& other)
{
    _impl = other._impl;
    return *this;
}

template <typename... values_t>
event<values_t...>::event(event&& other)
{
    *this = std::move(other);
}

template <typename... values_t>
event<values_t...>& event<values_t...>::operator=(event<values_t...>&& other)
{
    _impl = std::move(other._impl);
    return *this;
}

template <typename... values_t>
event<values_t...>::~event()
{
    if (_impl.is_attached)
        detail::state.context.events.destroy_event_and_data(_impl);
}

template <typename... values_t>
event<values_t...>& event<values_t...>::merge(const event<values_t...>& other)
{
    detail::state.context.events.merge_events_and_datas<values_t...>(_impl, other._impl);
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::operator+=(const event<values_t...>& other)
{
    return merge(other);
}

template <typename... values_t>
event<values_t...>& event<values_t...>::attach()
{
    detail::state.context.events.attach_to_wrapper(_impl);
    return *this;
}

template <typename... values_t>
template <typename widget_t>
event<values_t...>& event<values_t...>::detach(widget_t& widget)
{
    detail::state.context.widgets.detach_to_widget(_impl, widget);
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::detach()
{
    detail::state.context.events.detach_to_registry(_impl);
    return *this;
}

template <typename... values_t>
event<values_t...>& event<values_t...>::on_trigger(const on_trigger_callback& trigger_callback)
{
    _impl.callbacks.push_back(trigger_callback);
    return *this;
}

template <typename... values_t>
const event<values_t...>& event<values_t...>::trigger(values_t&&... values) const
{
    detail::state.context.events.trigger_values(_impl, std::forward<values_t>(values)...);
    return *this;
}

template <typename... values_t>
void event<values_t...>::trigger(std::future<future_values>&& future_value)
{
    detail::state.context.events.trigger_future_value(_impl, std::move(future_value));
}

template <typename... values_t>
void event<values_t...>::trigger(const std::shared_future<future_values>& shared_future_value)
{
    detail::state.context.events.trigger_shared_future_value(_impl, shared_future_value);
}

template <typename... values_t>
std::vector<event::on_trigger_callback>& event<values_t...>::trigger_callbacks()
{
    return _impl.callbacks;
}

template <typename... values_t>
const std::vector<event::on_trigger_callback>& event<values_t...>::trigger_callbacks() const
{
    return _impl.callbacks;
}
#pragma endregion

template <typename value_t>
value_t lerp(detail::enable_if_lerpable_t<value_t>&& min_value, value_t&& max_value, const float t)
{
    return detail::lerp<value_t>(std::forward<value_t>(min_value), std::forward<value_t>(max_value), t);
}

#pragma region animation
template <typename value_t>
template <typename duration_unit_t>
animation<value_t>::animation(
    const curve& curved_shape,
    lerpable_value&& min_value,
    lerpable_value&& max_value,
    const unsigned int duration_count,
    const animation_mode mode)
    : _impl(detail::state.context.animations.make_animation_and_data<value_t>(detail::state.context.events))
{
    detail::state.context.animations.shape_animation(_impl, curved_shape._data);
    detail::state.context.animations.set_animation_min(_impl, std::forward<value_t>(min_value));
    detail::state.context.animations.set_animation_max(_impl, std::forward<value_t>(max_value));
    detail::state.context.animations.set_animation_duration<value_t, duration_unit_t>(_impl, duration_count);
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
    _impl = other._impl;
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
    _impl = std::move(other._impl);
    return *this;
}

template <typename value_t>
animation<value_t>::~animation()
{
    if (_impl.event.is_attached)
        detail::state.context.animations.destroy_animation_and_data(_impl);
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    detail::state.context.events.merge_events_and_datas(_impl.event, value_changed_event);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_tick(const on_tick_callback& tick_callback)
{
    _impl.event.callbacks.push_back(tick_callback);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::start()
{
    detail::state.context.animations.start_animation(_impl);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::stop()
{
    detail::state.context.animations.stop_animation(_impl);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::reset()
{
    detail::state.context.animations.reset_animation(_impl);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::shape(const curve& curved_shape)
{
    detail::state.context.animations.shape_animation(_impl, curved_shape._data);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::min(value_t&& min_value)
{
    detail::state.context.animations.set_animation_min(_impl, std::forward<value_t>(min_value));
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::max(value_t&& max_value)
{
    detail::state.context.animations.set_animation_max(_impl, std::forward<value_t>(max_value));
    return *this;
}

template <typename value_t>
template <typename duration_unit_t>
animation<value_t>& animation<value_t>::duration(const unsigned int count)
{
    detail::state.context.animations.set_animation_duration<value_t, duration_unit_t>(_impl, count);
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::attach()
{
    detail::state.context.animations.attach_animation(_impl);
    return *this;
}

template <typename value_t>
template <typename widget_t>
animation<value_t>& animation<value_t>::detach(widget_t& widget)
{
    detail::state.context.widgets.detach_animation(_impl, widget);
}

template <typename value_t>
animation<value_t>& animation<value_t>::detach()
{
    detail::state.context.animations.detach_animation(_impl);
    return *this;
}
#pragma endregion

#pragma region animatable
template <typename value_t>
animatable<value_t>::animatable(const animation<value_t>& animated_value)
{
    _data.is_animated = true;
    _data.animation = animated_value._impl;
}

template <typename value_t>
animatable<value_t>::animatable(const value_t& value)
{
    _data.is_animated = false;
    _data.value = value;
}

template <typename value_t>
animatable<value_t>::animatable(const animatable& other)
{
    *this = other;
}

template <typename value_t>
animatable<value_t>& animatable<value_t>::operator=(const animatable& other)
{
    _data = other._data;
    return *this;
}

template <typename value_t>
animatable<value_t>::animatable(animatable&& other)
{
    *this = std::move(other);
}

template <typename value_t>
animatable<value_t>& animatable<value_t>::operator=(animatable&& other)
{
    _data = std::move(other._data);
    return *this;
}

template <typename value_t>
template <typename widget_t>
void animatable<value_t>::assign(widget_t* widget, value_t& target_value) const
{
    if (_data.is_animated) {
        detail::animation_impl<value_t>& _animation = _data.animation.value().get();
        _animation.event.callbacks.emplace_back(([&target_value, widget](const value_t& _value_changed) {
            target_value = _value_changed;
            get_context().must_draw<widget_t>(widget);
        }));
        detail::state.context.animations.start_animation(_animation);
        detail::state.context.widgets.detach_animation(_animation, *widget);
    } else
        target_value = _data.value.value();
}
#pragma endregion

#pragma region context

template <typename widget_t>
context& context::must_draw(widget_t* widget)
{
    detail::state.context.widgets.must_draw(*widget);
    return *this;
}

template <typename widget_t>
context& context::destroy(widget_t& widget, const bool destroy_children)
{
    using namespace internal::impl;
    // destroy component
    // widgets_container.destroy()

    // destroy in tree
    // widgets_ptrs_container.

    return *this;
}
#pragma endregion

#pragma region main
template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    return detail::state.context.widgets.make<widget_t>(std::forward<widget_args_t>(widget_args)...);
}

template <typename widget_t, typename... children_widgets_t>
void declare(widget_t* widget, children_widgets_t&... children_widgets)
{
    detail::state.context.widgets.declare(widget, children_widgets...);
    if constexpr (detail::has_resolve<widget_t>)
        detail::state.context.widgets.on_resolve(widget, [&](const detail::constraint_data& constraints, detail::advice_data& advice) {
            // TODO
        });
    if constexpr (detail::has_draw<widget_t>)
        detail::state.context.widgets.on_draw(widget, [=](detail::command_data& command) { // [=] otherwise we pass a reference to ptr
            draw_command _hl_command(command);
            widget->draw(_hl_command);
        });
}

template <typename widget_t>
void on_resolve(widget_t* widget, const std::function<void(const resolve_constraint&, resolve_advice&)>& resolve_callback)
{
    detail::state.context.widgets.on_resolve(widget, [&](const detail::constraint_data& constraints, detail::advice_data& advice) {
        // TODO
    });
}

template <typename widget_t>
void on_draw(widget_t* widget, std::function<void(draw_command&)> draw_callback)
{
    detail::state.context.widgets.on_draw(widget, [=](detail::command_data& command) { // [=] otherwise we pass a reference to ptr
        draw_command _hl_command(command);
        draw_callback(_hl_command);
    });
}

template <typename widget_t>
void launch(widget_t& widget,
    const std::function<void()>& on_renderer_started) { detail::state.launch(widget, on_renderer_started); }

template <typename widget_t>
std::function<void()> launch_embedded(widget_t& widget) { return detail::state.attach(widget); }
#pragma endregion
}
