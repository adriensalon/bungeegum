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
    struct image : base_widget {

        image()
        {
            build_advanced(this, [this](build_advanced_context& context) {
                std::cout << "image\n";
            });
        }
    };

}
}