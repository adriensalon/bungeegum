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

    /// @brief A widget that displays an image.
    struct image_widget {
        constexpr static char* internal_name = "image";
        // bungeegum_WIDGET(image_widget)

        image_widget()
        {
            // register_widget(this);
            // std::cout << "image ctor\n";
            // build(this);
        }

        ~image_widget()
        {
            // std::cout << "image dtor\n";
        }

        float x = 0.f;

        image_widget& set(float xx)
        {
            x = xx;
            return *this;
        }
    };

}
}

#define image bungeegum::create<bungeegum::widgets::image_widget>