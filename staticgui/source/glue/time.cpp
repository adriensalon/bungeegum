//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <iomanip>
#include <sstream>

#include <staticgui/glue/time.hpp>

namespace staticgui {
namespace glue {

    timestamp::timestamp()
    {
        now();
    }

    timestamp::timestamp(const std::chrono::system_clock::time_point& time_point)
    {
        _time = std::chrono::system_clock::to_time_t(time_point);
    }

    timestamp::timestamp(const timestamp& other)
    {
        *this = other;
    }

    timestamp& timestamp::operator=(const timestamp& other)
    {
        _time = other._time;
        return *this;
    }

    timestamp::timestamp(timestamp&& other)
    {
        *this = std::move(other);
    }

    timestamp& timestamp::operator=(timestamp&& other)
    {
        _time = std::move(other._time);
        return *this;
    }

    void timestamp::now()
    {
        std::chrono::system_clock::time_point _tp = std::chrono::system_clock::now();
        _time = std::chrono::system_clock::to_time_t(_tp);
    }

    std::string timestamp::get(const std::string& format) const
    {
        std::tm _ltime;
        localtime_s(&_ltime, &_time);
        std::ostringstream _sstream;
        _sstream << std::put_time(&_ltime, format.c_str());
        return _sstream.str();
    }

    stopwatch::stopwatch()
    {
        _last = std::chrono::system_clock::now();
    }

    stopwatch::stopwatch(const stopwatch& other)
    {
        *this = other;
    }

    stopwatch& stopwatch::operator=(const stopwatch& other)
    {
        _last = other._last;
        return *this;
    }

    stopwatch::stopwatch(stopwatch&& other)
    {
        *this = std::move(other);
    }

    stopwatch& stopwatch::operator=(stopwatch&& other)
    {
        _last = std::move(other._last);
        return *this;
    }

}
}
