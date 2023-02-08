//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <array>
#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

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

        template <typename unit_t = std::chrono::milliseconds>
        unit_t lap_at_least(const unit_t at_least);

    private:
        std::chrono::system_clock::time_point _last;
    };

    template <unsigned int count_t, typename unit_t = std::chrono::milliseconds>
    struct chronometer {
        struct frame {
            std::vector<unsigned int> indices;
            std::vector<float> ratios;
        };

        struct task {
            stopwatch watch;
            unit_t duration;
            unsigned int count = 0;
        };

        void new_frame();
        void begin(const std::string& name);
        void end(const std::string& name);
        const task& get_task(const std::string& name);
        const std::vector<task>& get_tasks();
        const std::array<frame, count_t>& get_frames();
        void on_new_task(const std::function<void(const std::string&, unsigned int)>& new_task_callback);
        void on_new_frame(const std::function<void()>& new_frame_callback);

    private:
        unit_t _frame_duration;
        std::vector<std::function<void(const std::string&, unsigned int)>> _new_task_callbacks;
        std::vector<std::function<void()>> _new_frame_callbacks;
        std::vector<task> _tasks;
        std::array<frame, count_t> _frames;
        std::unordered_map<std::string, unsigned int> _task_names;
    };

}
}

#include <staticgui/glue/time.inl>
