#pragma once

#include <string>

namespace bungeegum {
namespace detail {

    /// @brief
    enum struct console_color {
        default,
        blue,
        green,
        cyan,
        red,
        magenta,
        yellow
    };

    /// @brief
    void console_log(const std::string& message, const console_color color = console_color::default);
}
}
