//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <algorithm>

#include <staticgui/glue/toolchain.hpp>

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

    template <unsigned int count_t, typename unit_t>
    void chronometer<count_t, unit_t>::new_frame()
    {
        if constexpr (is_build_debug) {
            // std::cout << " --- new frame \n";
            for (auto _k = 0; _k < _tasks.size(); _k++) {
                task& _task = _tasks[_k];
                if (_task.count > 0) {
                    // std::cout << "k = " << _k << " --- task duration = " << std::chrono::duration_cast<std::chrono::nanoseconds>(_task.duration).count() << std::endl;
                    _frames[count_t - 1].indices.push_back(_k);
                    _frames[count_t - 1].ratios.push_back(static_cast<float>(_task.duration.count()) / _frame_duration.count());
                    // std::cout << "ratio = " << _frames[count_t - 1].ratios[_k];
                    _task.count = 0;
                    _task.duration = unit_t(0);
                }
            }
            std::iter_swap(_frames.data(), _frames.data() + count_t - 1);
            _frames[count_t - 1].indices.clear();
            _frames[count_t - 1].ratios.clear();
        }
    }

    template <unsigned int count_t, typename unit_t>
    void chronometer<count_t, unit_t>::begin(const std::string& name)
    {
        if constexpr (is_build_debug) {
            if (_task_names.find(name) == _task_names.end()) {
                _task_names.emplace(name, static_cast<unsigned int>(_tasks.size()));
                _tasks.emplace_back();
            }
            task& _task = _tasks[_task_names[name]];
            // _task.watch.lap<unit_t>(); // <<<<<<<<<<<<<<<<<<<<
            _task.count++;
        }
    }

    template <unsigned int count_t, typename unit_t>
    void chronometer<count_t, unit_t>::end(const std::string& name)
    {
        if constexpr (is_build_debug) {
            task& _task = _tasks[_task_names[name]];
            auto _duration = _task.watch.lap<unit_t>();
            _task.duration += _duration;
            _frame_duration += _duration;
        }
    }

    template <unsigned int count_t, typename unit_t>
    const typename chronometer<count_t, unit_t>::task& chronometer<count_t, unit_t>::get_task(const std::string& name)
    {
        return _tasks[_task_names[name]];
    }

    template <unsigned int count_t, typename unit_t>
    const std::vector<typename chronometer<count_t, unit_t>::task>& chronometer<count_t, unit_t>::get_tasks()
    {
        return _tasks;
    }

    template <unsigned int count_t, typename unit_t>
    const std::array<typename chronometer<count_t, unit_t>::frame, count_t>& chronometer<count_t, unit_t>::get_frames()
    {
        return _frames;
    }

}
}
