//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>
#include <optional>
#include <string>

#include <bungeegum/glue/backtrace.hpp>

namespace bungeegum {
namespace detail {

#if defined(bungeegum_BACKTRACE_SIZE)
    constexpr unsigned int backtrace_size_v = bungeegum_BACKTRACE_SIZE;
#else
    constexpr unsigned int backtrace_size_v = 10;
#endif

    void throw_library_bad_usage(const std::string& what);
    void throw_library_bad_implementation(const std::string& what);
    void protect_library(const std::function<void()>& try_callback);

    void protect_userspace(const std::function<void()>& try_callback);
    bool has_userspace_thrown();
    std::optional<backtraced_exception>& get_userspace_thrown_exception();
}

/// @brief
/// @details
/// @param what
void throw_error(const std::string& what);

}
