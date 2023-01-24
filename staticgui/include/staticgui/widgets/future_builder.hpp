//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <atomic>
#include <future>
#include <optional>
#include <type_traits>

#include <staticgui/staticgui.hpp>

namespace staticgui {
namespace widgets {

    /// @brief A widget that replaces its content by another widget based on the result of a 'std::future'
    /// @details
    struct future_builder : base_widget {

        /// @brief
        /// @tparam wait_widget_t
        /// @tparam complete_callback_t
        /// @tparam value_t
        /// @param future
        /// @param wait
        /// @param complete_callback
        template <typename value_t, typename wait_widget_t, typename complete_callback_t>
        future_builder(std::future<value_t>& future, const wait_widget_t& wait, complete_callback_t complete_callback)
        {
            build_stateful(this, [&](build_context& context) { 
				std::future<void> _future_tracker = std::async([&]() { // out of scope ???
					future.wait();
					if constexpr (std::is_void_v<value_t>)
						context.replace(wait, complete_callback());
					else 
						context.replace(wait, complete_callback(std::forward<value_t>(future.get()))); });
				return wait; });
        }

        // do something in the desctructor ?

    private:
        // std::future<void> _future_tracker;
    };

}
}