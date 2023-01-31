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

        struct animation_impl {
        };

        static animation_impl _NO;
    }
}

template <typename value_t>
animation<value_t>::animation(const curve<value_t>& bezier_curve)
    : _impl(internal::impl::_NO)
{
}

template <typename value_t>
animation<value_t>& animation<value_t>::on_value_changed(const event<value_t>& value_changed_event)
{
    std::cout << "YES 2 \n";
    return *this;
}

template <typename value_t>
template <typename other_value_t>
animation<value_t>& animation<value_t>::start_after(const animation<other_value_t>& previous)
{
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