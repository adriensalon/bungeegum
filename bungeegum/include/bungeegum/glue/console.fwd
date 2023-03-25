#pragma once

#include <string>
#include <vector>

namespace bungeegum {
namespace detail {

    enum struct console_color {
        default,
        blue,
        green,
        cyan,
        red,
        magenta,
        yellow
    };

    void console_log(const std::string& message, const console_color color = console_color::default);

    struct console_command {
        std::string name;
        std::vector<std::string> args;
    };

    std::vector<console_command> console_args(int argc, char* argv[]);

}
}
