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
value<value_t>::value(const animation<traits::lerpable_value_t<value_t>>& animated_value)
{
    _is_animated = true;
    _value = animated_value;
}

template <typename value_t>
value<value_t>::value(const traits::lerpable_value_t<value_t>& static_value)
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

}