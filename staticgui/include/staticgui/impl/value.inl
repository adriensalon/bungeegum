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
namespace internal {
    namespace impl {

        struct value_impl {
            bool _is_animated;
            // std::variant<value_t, animation<value_t>> _value;
        };

        static value_impl _NO3;
    }
}

template <typename value_t>
value<value_t>::value(const animation<traits::lerpable_value_t<value_t>>& animated_value)
    : _impl(internal::impl::_NO3)
{
}

template <typename value_t>
value<value_t>::value(const traits::lerpable_value_t<value_t>& static_value)
    : _impl(internal::impl::_NO3)
{
}

template <typename value_t>
void value<value_t>::assign(value_t& target_value) const
{
}

}