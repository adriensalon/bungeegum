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

        auto create(build_context& context)
        {
            return *this;
        };

        void update(build_context& context)
        {
        }

        float _x = 0.f;
        image()
        {
            build2(this);
        }
    };

}
}