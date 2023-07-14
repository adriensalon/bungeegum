#pragma once

#include <algorithm>
#include <thread>

#include <bungeegum/glue/toolchain.hpp>

namespace bungeegum {
namespace detail {

    template <typename unit_t>
    unit_t stopwatch::lap()
    {
        std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
        unit_t _duration = std::chrono::duration_cast<unit_t>(_now - _last);
        _last = _now;
        return _duration;
    }

    template <typename unit_t>
    unit_t stopwatch::lap_at_least(const unit_t at_least)
    {
        std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
        unit_t _duration = std::chrono::duration_cast<unit_t>(_now - _last);
        if (_duration < at_least) {
            std::this_thread::sleep_for(at_least - _duration);
            _now = std::chrono::system_clock::now();
            _duration = at_least;
        }
        _last = _now;
        return _duration;
    }

    template <std::size_t count_t, typename unit_t>
    void chronometer<count_t, unit_t>::new_frame()
    {
        for (std::size_t _k = 0; _k < _tasks.size(); _k++) {
            task& _task = _tasks[_k];
            _frames[count_t - 1].indices.push_back(_k);
            _frames[count_t - 1].durations.push_back(_task.duration.count());
        }
        for (std::function<void()>& _callback : _new_frame_callbacks)
            _callback();
        for (std::size_t _k = 0; _k < _tasks.size(); _k++) {
            task& _task = _tasks[_k];
            _task.instances = 0;
            _task.duration = unit_t(0);
        }
        std::iter_swap(_frames.data(), _frames.data() + count_t - 1);
        _frames[count_t - 1].indices.clear();
        _frames[count_t - 1].durations.clear();
    }

    template <std::size_t count_t, typename unit_t>
    void chronometer<count_t, unit_t>::begin_task(const std::string& name)
    {
        if (_task_names.find(name) == _task_names.end()) {
            _task_names.emplace(name, static_cast<std::size_t>(_tasks.size()));
            _tasks.emplace_back();
            for (std::function<void(const std::string&, size_t)>& _callback : _new_task_callbacks)
                _callback(name, static_cast<std::size_t>(_tasks.size()) - 1);
        }
        task& _task = _tasks[_task_names[name]];
        _task.watch.lap<unit_t>();
        _task.instances++;
    }

    template <std::size_t count_t, typename unit_t>
    void chronometer<count_t, unit_t>::end_task(const std::string& name)
    {
        task& _task = _tasks[_task_names[name]];
        unit_t _duration = _task.watch.lap<unit_t>();
        _task.duration += _duration;
        _frame_duration += _duration;
    }

    template <std::size_t count_t, typename unit_t>
    const typename chronometer<count_t, unit_t>::task& chronometer<count_t, unit_t>::get_task(const std::string& name)
    {
        return _tasks[_task_names[name]];
    }

    template <std::size_t count_t, typename unit_t>
    const std::vector<typename chronometer<count_t, unit_t>::task>& chronometer<count_t, unit_t>::get_tasks()
    {
        return _tasks;
    }

    template <std::size_t count_t, typename unit_t>
    const std::array<typename chronometer<count_t, unit_t>::frame, count_t>& chronometer<count_t, unit_t>::get_frames()
    {
        return _frames;
    }

    template <std::size_t count_t, typename unit_t>
    void chronometer<count_t, unit_t>::on_new_task(const std::function<void(const std::string&, std::size_t)>& new_task_callback)
    {
        _new_task_callbacks.push_back(new_task_callback);
    }

    template <std::size_t count_t, typename unit_t>
    void chronometer<count_t, unit_t>::on_new_frame(const std::function<void()>& new_frame_callback)
    {
        _new_frame_callbacks.push_back(new_frame_callback);
    }
}
}
