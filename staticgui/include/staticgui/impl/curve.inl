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

        struct runtime_curve_component {
        };

        struct curve_impl {
        };

        static curve_impl _NO2;
    }
}

template <typename value_t>
curve<value_t>::curve(const traits::lerpable_value_t<value_t>& min, const value_t& max)
    : _impl(internal::impl::_NO2)
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