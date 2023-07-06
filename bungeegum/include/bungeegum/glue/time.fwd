#pragma once

#include <chrono>
#include <ctime>

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
        template <typename unit_t = std::chrono::milliseconds>
        unit_t lap();

        /// @brief Gets the duration between now and the last time we retrieved a duration,
        /// blocking the thread until a certain duration has passed since the last time we
        /// retrieved a duration.
        /// @tparam unit_t is the type from std::chrono to use as units.
        template <typename unit_t = std::chrono::milliseconds>
        unit_t lap_at_least(const unit_t at_least);

    private:
        std::chrono::system_clock::time_point _last;
    };

    // template <unsigned int count_t, typename unit_t = std::chrono::milliseconds>
    // struct chronometer {
    //     struct frame {
    //         std::vector<unsigned int> indices;
    //         std::vector<float> ratios;
    //     };

    //     struct task {
    //         stopwatch watch;
    //         unit_t duration;
    //         unsigned int count = 0;
    //     };

    //     void new_frame();
    //     void begin(const std::string& name);
    //     void end(const std::string& name);
    //     const task& get_task(const std::string& name);
    //     const std::vector<task>& get_tasks();
    //     const std::array<frame, count_t>& get_frames();
    //     void on_new_task(const std::function<void(const std::string&, unsigned int)>& new_task_callback);
    //     void on_new_frame(const std::function<void()>& new_frame_callback);

    // private:
    //     unit_t _frame_duration;
    //     std::vector<std::function<void(const std::string&, unsigned int)>> _new_task_callbacks;
    //     std::vector<std::function<void()>> _new_frame_callbacks;
    //     std::vector<task> _tasks;
    //     std::array<frame, count_t> _frames;
    //     std::unordered_map<std::string, unsigned int> _task_names;
    // };

}
}

#include <bungeegum/glue/time.inl>
