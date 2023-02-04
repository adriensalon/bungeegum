//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <string>
#include <vector>

namespace staticgui {
namespace glue {

    enum class console_color {
        black,
        blue,
        green,
        cyan,
        red,
        magenta,
        yellow
    };

    void console_log(const std::string& message, const console_color color = console_color::black, const bool newline = true);

#if !defined(__EMSCRIPTEN__)

    struct console_command {
        std::string name;
        std::vector<std::string> args;
    };

    std::vector<console_command> console_args(int argc, char* argv[]);

#endif
}
}
