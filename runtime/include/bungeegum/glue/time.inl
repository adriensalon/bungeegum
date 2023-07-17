#pragma once

#include <algorithm>
#include <thread>

#include <bungeegum/glue/backtrace.hpp>
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

    template <typename unit_t>
    void chronometer<unit_t>::new_frame()
    {
        _frame_duration = { 0 };
        for (std::size_t _k = 0; _k < _tasks.size(); _k++) {
            chronometer_task<unit_t>& _task = _tasks[_k];
            for (const task_callback& _callback : _new_frame_for_each_task_callbacks) {
                _callback(_task);
            }
            _frame_duration += _task.duration;
            _task.duration = unit_t { 0 };
        }
    }

    template <typename unit_t>
    void chronometer<unit_t>::begin_task(const std::string& name)
    {
        if (_task_names.find(name) == _task_names.end()) {
            _task_names.emplace(name, static_cast<std::size_t>(_tasks.size()));
            chronometer_task<unit_t>& _task = _tasks.emplace_back();
            _task.name = name;
            for (const task_callback& _callback : _new_task_callbacks) {
                _callback(_task);
            }
            return;
        }
        chronometer_task<unit_t>& _task = _tasks.at(_task_names.at(name));
        if (_task._is_running) {
            throw backtraced_exception("This chronometer task has already been started with the "
                                       "begin_task() method. Please use the end_task() method "
                                       "before starting it again.");
        }
        _task._watch.lap<unit_t>();
        _task._is_running = true;
    }

    template <typename unit_t>
    void chronometer<unit_t>::end_task(const std::string& name)
    {
        chronometer_task<unit_t>& _task = _tasks.at(_task_names.at(name));
        if (!_task._is_running) {
            throw backtraced_exception("This chronometer task has already been ended with the "
                                       "end_task() method. Please use the begin_task() method "
                                       "before ending it again.");
        }
        unit_t _duration = _task._watch.lap<unit_t>();
        _task.duration += _duration;
        _task._is_running = false;
    }

    template <typename unit_t>
    unit_t chronometer<unit_t>::frame_duration() const
    {
        return _frame_duration;
    }

    template <typename unit_t>
    void chronometer<unit_t>::on_new_task(const chronometer<unit_t>::task_callback& new_task_callback)
    {
        _new_task_callbacks.push_back(new_task_callback);
    }

    template <typename unit_t>
    void chronometer<unit_t>::on_new_frame_for_each_task(const chronometer<unit_t>::task_callback& new_frame_for_each_task_callback)
    {
        _new_frame_for_each_task_callbacks.push_back(new_frame_for_each_task_callback);
    }
}
}
