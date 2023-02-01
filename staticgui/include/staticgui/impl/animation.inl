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

}