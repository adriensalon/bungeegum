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
#include <ctime>
#include <iostream>

namespace staticgui {
namespace glue {

    struct timestamp {
        timestamp();
        timestamp(const std::chrono::system_clock::time_point& time_point);
        timestamp(const timestamp& other);
        timestamp& operator=(const timestamp& other);
        timestamp(timestamp&& other);
        timestamp& operator=(timestamp&& other);

        void now();
        std::string get(const std::string& format = "%H:%M:%S") const;

    private:
        std::time_t _time;
    };

    struct stopwatch {
        stopwatch();
        stopwatch(const stopwatch& other);
        stopwatch& operator=(const stopwatch& other);
        stopwatch(stopwatch&& other);
        stopwatch& operator=(stopwatch&& other);

        template <typename unit_t = std::chrono::milliseconds>
        unit_t lap();

    private:
        std::chrono::system_clock::time_point _last;
    };

}
}

#include <staticgui/glue/time.inl>
