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
    struct gesture_detector : base_widget {

        gesture_detector()
        {
            std::cout << "gesture\n";
            build_stateless(this, image());
            // build_advanced(this, [this](build_advanced_context& context) {
            //     std::cout << "gesture\n";
            // });
        }

        gesture_detector on_tap(std::function<void()> tap_callback)
        {
            return *this;
        }
    };

}
}