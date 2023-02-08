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
#include <string>

#include <staticgui/glue/backtrace.hpp>

namespace staticgui {
namespace detail {

    void throw_library_bad_usage(const std::string& what);
    void throw_library_bad_implementation(const std::string& what);
    void protect_library(const std::function<void()>& try_callback);

    void throw_user_bad_implementation(const std::string& what);
    void protect_userspace(const std::function<void()>& try_callback);
    bool has_userspace_thrown();
    std::optional<glue::backtraced_exception>& get_userspace_thrown_exception();
}
}
