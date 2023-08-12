#pragma once

#include <chrono>
#include <cstddef>
#include <ctime>
#include <functional>
#include <unordered_map>
#include <vector>

#if !defined(BUNGEEGUM_USE_TIME_UNIT)
#define BUNGEEGUM_USE_TIME_UNIT std::chrono::milliseconds
#endif

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct represent mutable time points that can be updated and
    /// retrieved as a formatted string.
    /// @details Instances of this struct can be copied (performing deep copy) and moved.
    struct timestamp {
        timestamp();
        timestamp(const timestamp& other) = default;
        timestamp& operator=(const timestamp& other) = default;
        timestamp(timestamp&& other) = default;
        timestamp& operator=(timestamp&& other) = default;

        /// @brief Creates an instance from a std::chrono time point.
        timestamp(const std::chrono::system_clock::time_point& time_point);

        /// @brief Sets this instance to the current time point.
        void now();

        /// @brief Gets a formatted string of the last time point recorded on creation of this
        /// instance or the last time now() has been called.
        std::string formatted(const std::string& format = "%H:%M:%S") const;

    private:
        std::time_t _time;
    };

    /// @brief Instances of this struct represent mutable timers that measure durations in a
    /// stopwatch manner : we can only get a duration between now and the last time we retrieved
    /// a duration.
    /// @details Instances of this struct can be copied (performing deep copy) and moved.
    struct stopwatch {
        stopwatch();
        stopwatch(const stopwatch& other) = default;
        stopwatch& operator=(const stopwatch& other) = default;
        stopwatch(stopwatch&& other) = default;
        stopwatch& operator=(stopwatch&& other) = default;

        /// @brief Gets the duration between now and the last time we retrieved a duration.
        /// @tparam unit_t is the type from std::chrono to use as units.
        template <typename unit_t = BUNGEEGUM_USE_TIME_UNIT>
        unit_t lap();

        /// @brief Gets the duration between now and the last time we retrieved a duration,
        /// blocking the thread until a certain duration has passed since the last time we
        /// retrieved a duration.
        /// @tparam unit_t is the type from std::chrono to use as units.
        template <typename unit_t = BUNGEEGUM_USE_TIME_UNIT>
        unit_t lap_at_least(const unit_t at_least);

    private:
        std::chrono::system_clock::time_point _last;
    };

    /// @brief Instances of this struct represent a task which duration is measured.
    /// @details Instances of this struct can be copied (deep copy) or moved.
    /// @tparam unit_t is the type from std::chrono to use as units.
    template <typename unit_t = BUNGEEGUM_USE_TIME_UNIT>
    struct chronometer_task {
        std::string name = {};
        unit_t duration = unit_t { 0 };

    private:
        template <typename _unit_t>
        friend struct chronometer;
        bool _is_running = true;
        stopwatch _watch = {};
    };

    /// @brief Instances of this struct represent an array of tasks that can be started or stopped
    /// multiple times per frame.
    /// @details Instances of this struct can be copied (deep copy) or moved.
    /// @tparam unit_t is the type from std::chrono to use as units.
    template <typename unit_t = BUNGEEGUM_USE_TIME_UNIT>
    struct chronometer {

        /// @brief Defines the callback type for events.
        using task_callback = std::function<void(const chronometer_task<unit_t>&)>;

        /// @brief Must be called at the beginning of every frame.
        void new_frame();

        /// @brief Begins a task with the specified name.
        /// @exception Throws a backtraced_exception if the task has already been started.
        void begin_task(const std::string& name);

        /// @brief Ends a task with the specified name.
        /// @exception Throws a backtraced_exception if the task has already been ended.
        void end_task(const std::string& name);

        ///
        unit_t frame_duration() const;

        /// @brief Registers a callback to be called each time a new task is created.
        void on_new_task(const task_callback& new_task_callback);

        /// @brief Registers a callback to be called each frame for each task.
        void on_new_frame_for_each_task(const task_callback& new_frame_for_task_callback);

    private:
        unit_t _frame_duration = {};
        std::vector<task_callback> _new_task_callbacks = {};
        std::vector<task_callback> _new_frame_for_each_task_callbacks = {};
        std::unordered_map<std::string, std::size_t> _task_names = {};
        std::vector<chronometer_task<unit_t>> _tasks = {};
    };
}
}

#include <bungeegum/glue/time.inl>
