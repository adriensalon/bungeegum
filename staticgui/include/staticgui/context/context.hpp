//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <chrono>

namespace staticgui {

namespace detail {
    bool tick(const std::chrono::milliseconds& delta_time);
    void draw();
}

/// @brief
/// @param context
void use(const std::string& context = "default");

/// @brief
/// @return
const std::string& used();

}
