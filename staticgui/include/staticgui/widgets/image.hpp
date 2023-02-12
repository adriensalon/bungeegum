//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/staticgui.hpp>

namespace staticgui {
namespace widgets {

    /// @brief A widget that displays an image.
    struct image_widget {
        constexpr static char* internal_name = "image";
        // STATICGUI_WIDGET(image_widget)

        image_widget()
        {
            // std::cout << "image ctor\n";
            build(this);
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

#define image staticgui::create<staticgui::widgets::image_widget>