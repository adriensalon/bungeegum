//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {
namespace glue {

    template <typename unit_t>
    stopwatch<unit_t>::stopwatch()
    {
        _last = std::chrono::system_clock::now();
    }

    template <typename unit_t>
    stopwatch<unit_t>::stopwatch(stopwatch&& other)
    {
        *this = std::move(other);
    }

    template <typename unit_t>
    stopwatch<unit_t>& stopwatch<unit_t>::operator=(stopwatch&& other)
    {
        _last = std::move(other._last);
        return *this;
    }

    template <typename unit_t>
    unit_t stopwatch<unit_t>::lap()
    {
        auto _now = std::chrono::system_clock::now();
        auto _duration = std::chrono::duration_cast<unit_t>(_now - _last);
        _last = _now;
        return _duration;
    }

}
}
