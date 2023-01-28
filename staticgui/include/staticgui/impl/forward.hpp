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
    namespace detail {

        struct event_impl;

        struct curve_impl;

        struct animation_impl;

        struct value_impl;
    }
}

struct color;

template <typename... values_t>
struct event;

template <typename value_t>
struct curve;

template <typename... values_t>
struct animation;

template <typename value_t>
struct value;

struct vrai_context;

}