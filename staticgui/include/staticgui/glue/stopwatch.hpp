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
namespace glue {

    template <typename unit_t = std::chrono::milliseconds>
    struct stopwatch {
        stopwatch();
        stopwatch(const stopwatch& other) = delete;
        stopwatch& operator=(const stopwatch& other) = delete;
        stopwatch(stopwatch&& other);
        stopwatch& operator=(stopwatch&& other);

        unit_t lap();

    private:
        std::chrono::system_clock::time_point _last;
    };

}
}

#include <staticgui/glue/stopwatch.inl>
