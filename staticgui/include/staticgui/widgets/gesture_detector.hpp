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
    struct gesture_detector {

        gesture_detector()
        {
            build(this, [&](advanced_context& ctx) {

            });
        }

        gesture_detector& on_tap(std::function<void()> tap_callback)
        {
            return *this;
        }

        STATICGUI_WIDGET(gesture_detector)
    };

}
}