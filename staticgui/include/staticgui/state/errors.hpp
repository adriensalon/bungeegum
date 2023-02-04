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

    void throw_userspace(const std::string& what);
    void throw_bad_usage(const std::string& what);
    void throw_bad_implementation(const std::string& what);

    // invoke autour du user code
    // catch from nullptr, from user, from bad usage -> ui display + TRACED pour user et bad usage
    void try_catch_user_space(const bool rethrow, const std::function<void()>& try_callback, const std::function<void()>& catch_callback = nullptr);
    // -> 2 functions
    // - 1.

    bool has_user_space_caught();
    void display_user_space_gui();

    // invoke sur tout le programme
    // catch from nullptr, from bad implementation  -> console message TRACED pour bad implementation
    void try_catch_library_space(const std::function<void()>& try_callback);
}
}
