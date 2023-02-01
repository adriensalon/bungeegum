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
event<values_t...>::event(const std::function<void(values_t...)>& trigger_callback)
    : _impl(detail::global_events.make_event_and_data<values_t...>())
{
}

template <typename value_t>
animation<value_t>::animation(const curve<value_t>& bezier_curve)
    : _impl(detail::global_animations.make_animation_and_data<value_t>())
{
}

template <typename value_t>
animation<value_t>::animation(const animation<value_t>& other)
    : _impl(detail::global_animations.make_animation_and_data<value_t>())
{
}

template <typename value_t>
animation<value_t>& animation<value_t>::operator=(const animation<value_t>& other)
{
    return *this;
}

template <typename value_t>
animation<value_t>::animation(animation<value_t>&& other)
    : _impl(detail::global_animations.make_animation_and_data<value_t>())
{
}

template <typename value_t>
animation<value_t>& animation<value_t>::operator=(animation<value_t>&& other)
{
    return *this;
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    std::cout << "YES 2 \n";
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

template <typename widget_t, typename... widget_args_t>
widget_t& make(widget_args_t&&... widget_args)
{
    return detail::global_widgets.make<widget_t>(std::forward<widget_args_t>(widget_args)...);
}

template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget)
{
    detail::global_widgets.build(widget, nullptr, child_widget);
}

template <typename widget_t, typename... children_widgets_t>
void build_advanced(widget_t* widget, std::function<void(layout&)> paint_callback, children_widgets_t&... children)
{
    detail::global_widgets.build(
        widget, [](detail::layout_state&) {}, children...);
}

template <typename widget_t>
void launch(widget_t& widget)
{
    detail::global_widgets.build_root(widget);

    std::cout << "application" << std::endl;
    detail::global_widgets.iterate_datas([&](detail::widget_data& _widget_data) {
        unsigned int _depth = detail::global_widgets.get_depth(_widget_data);
        for (unsigned int _k = 0; _k < _depth; _k++)
            std::cout << "   ";
        std::cout << "|__ ";
        std::cout << _widget_data.kind->name();
        if (_widget_data.paint_callback)
            std::cout << " [painter]";
        std::cout << std::endl;
    });

    detail::global_events.iterate_datas([&](detail::event_data& _event_data) {
        std::cout << "- event <";
        for (auto& _k : _event_data.kinds)
            std::cout << _k.name() << ", ";
        std::cout << "> \n";
    });

    detail::global_animations.iterate_datas([&](detail::animation_data& _animation_data) {
        std::cout << "- animation <";
        std::cout << _animation_data.kind->name() << "> \n";
    });

    // while (true) {
    //     print_tree<widget_impl*>(widgets_ptrs_container);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // }
}

template <typename widget_t>
void attach(widget_t& widget)
{
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

namespace internal {
    namespace impl {

        struct runtime_curve_component {
        };

        struct curve_impl {
        };

        static curve_impl _NO2;
    }
}

template <typename value_t>
curve<value_t>::curve(const detail::enable_if_lerpable_t<value_t>& min, const value_t& max)
// : _impl(internal::impl::_NO2)
{
}

template <typename value_t>
std::vector<std::pair<float, value_t>>& curve<value_t>::get_points()
{
}

template <typename value_t>
const std::vector<std::pair<float, value_t>>& curve<value_t>::get_points() const
{
}

template <typename value_t>
value_t curve<value_t>::get_value(const float t)
{
}

}