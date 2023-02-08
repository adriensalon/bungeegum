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
    unit_t stopwatch::lap()
    {
        auto _now = std::chrono::system_clock::now();
        auto _duration = std::chrono::duration_cast<unit_t>(_now - _last);
        _last = _now;
        return _duration;
    }

    template <typename unit_t>
    unit_t stopwatch::lap_at_least(const unit_t at_least)
    {
        auto _now = std::chrono::system_clock::now();
        auto _duration = std::chrono::duration_cast<unit_t>(_now - _last);
        if (_duration < at_least) {
            std::this_thread::sleep_for(at_least - _duration);
            _now = std::chrono::system_clock::now();
            _duration = at_least;
        }
        _last = _now;
        return _duration;
    }

}
}
