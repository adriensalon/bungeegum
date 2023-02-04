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

// lib -> catch bad implementation

// user -> catch bad usage, user, nullptr, std::exception, int, const char* et record un callback

namespace staticgui {
namespace detail {

    void throw_library_bad_usage(const std::string& what);
    void throw_library_bad_implementation(const std::string& what);
    void protect_library(const std::function<void()>& try_callback);

    void throw_userspace(const std::string& what);
    void protect_userspace(const std::function<void()>& try_callback);
    bool has_userspace_thrown();
    void display_userspace_gui();
}
}
