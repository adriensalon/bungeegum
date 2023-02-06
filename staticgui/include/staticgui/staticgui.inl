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
event<values_t...>& event<values_t...>::attach()
{
    detail::state.context.events.attach_to_wrapper(_impl);
    return *this;
}

template <typename... values_t>
template <typename widget_t>
event<values_t...>& event<values_t...>::detach(widget_t& widget)
{
    detail::state.context.widgets.attach_event(_impl, widget);
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
void event<values_t...>::trigger(std::future<typename detail::value_or_tuple<values_t...>::type>&& future_value)
{
    detail::state.context.events.trigger_future_value(_impl, std::move(future_value));
}

template <typename... values_t>
void event<values_t...>::trigger(const std::shared_future<typename detail::value_or_tuple<values_t...>::type>& shared_future_value)
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

#pragma region animation
template <typename value_t>
animation<value_t>::animation(const curve& bezier_curve)
    : _impl(detail::state.context.animations.make<value_t>())
{
}

template <typename value_t>
animation<value_t>::animation(const animation<value_t>& other)
    : _impl(detail::state.context.animations.make<value_t>())
{
}

template <typename value_t>
animation<value_t>& animation<value_t>::operator=(const animation<value_t>& other)
{
    return *this;
}

template <typename value_t>
animation<value_t>::animation(animation<value_t>&& other)
    : _impl(detail::state.context.animations.make<value_t>())
{
}

template <typename value_t>
animation<value_t>& animation<value_t>::operator=(animation<value_t>&& other)
{
    return *this;
}

template <typename value_t>
animation<value_t>::~animation()
{
    // if (_impl.is_attached)
    //     detail::state.context.events.destroy_event_and_data(_impl)
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    // std::cout << "YES 2 \n";
    return *this;
}

template <typename value_t>
void animation<value_t>::start()
{
}

template <typename value_t>
void animation<value_t>::stop()
{
}
#pragma endregion

#pragma region value
template <typename value_t>
value<value_t>::value(const animation<detail::enable_if_lerpable_t<value_t>>& animated_value)
{
    _is_animated = true;
    _value = animated_value;
}

template <typename value_t>
value<value_t>::value(const detail::enable_if_lerpable_t<value_t>& static_value)
{
    _is_animated = false;
    _value = static_value;
}

template <typename value_t>
void value<value_t>::assign(value_t& target_value) const
{
    if (_is_animated) {
        animation<value_t>& _animation = std::get<animation<value_t>>(_value);
        _animation.on_value_changed(event<value_t>([&](const value_t& _value_changed) {
            target_value = _value_changed;
        }));
    } else
        target_value = std::get<value_t>(_value);
}
#pragma endregion

#pragma region context
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
        detail::state.context.widgets.on_draw(widget, [&](detail::command_data& command) {
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
void on_draw(widget_t* widget, const std::function<void(draw_command&)>& draw_callback)
{
    detail::state.context.widgets.on_draw(widget, [&](detail::command_data& command) {
        draw_command _hl_command(command);
        draw_callback(_hl_command);
    });
}

template <typename widget_t>
void launch(widget_t& widget) { detail::state.launch(widget); }

template <typename widget_t>
std::function<void()> launch_embedded(widget_t& widget) { return detail::state.attach(widget); }
#pragma endregion
}
