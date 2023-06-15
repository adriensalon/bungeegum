//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    template <typename item_widget_t>
    struct list_view : base_widget {
        list_view(std::initializer_list<item_widget_t> children)
        {
            // build(children);
        }
    };
}
}