//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <iostream>

#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/toolchain.hpp>

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
#include <windows.h>
#endif

namespace bungeegum {
namespace detail {

    void console_log(const std::string& message, const console_color color)
    {
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
        HANDLE _handle;
#endif
        if constexpr (is_platform_win32_v || is_platform_uwp_v) {
            _handle = GetStdHandle(STD_OUTPUT_HANDLE);
            int _color = 7;
            if (color == console_color::blue)
                _color = 1;
            else if (color == console_color::green)
                _color = 2;
            else if (color == console_color::cyan)
                _color = 3;
            else if (color == console_color::red)
                _color = 4;
            else if (color == console_color::magenta)
                _color = 5;
            else if (color == console_color::yellow)
                _color = 6;
            SetConsoleTextAttribute(_handle, static_cast<WORD>(_color));
        } else if constexpr (is_platform_linux_v || is_platform_macos_v) {
            std::string _color = "\033[0m";
            if (color == console_color::blue)
                _color = "\033[0;34m";
            else if (color == console_color::green)
                _color = "\033[0;32m";
            else if (color == console_color::cyan)
                _color = "\033[0;36m";
            else if (color == console_color::red)
                _color = "\033[0;31m";
            else if (color == console_color::magenta)
                _color = "\033[0;35m";
            else if (color == console_color::yellow)
                _color = "\033[0;33m";
            std::cout << _color;
        }
        std::cout << message;
        if constexpr (is_platform_win32_v || is_platform_uwp_v)
            SetConsoleTextAttribute(_handle, 7);
        else if constexpr (is_platform_linux_v || is_platform_macos_v)
            std::cout << "\033[0m";
    }

    std::vector<console_command> console_args(int argc, char* argv[])
    {
        auto _result = std::vector<console_command>();
        auto _raw_options = std::vector<std::string>(argv + 1, argv + argc);
        if constexpr (is_platform_emscripten_v) {
            // todo
        } else {
            for (auto& raw_option : _raw_options) {
                if (!raw_option.empty()) {
                    if (raw_option[0] != '-') {
                        if (_result.empty())
                            return {};
                        _result.back().args.emplace_back(raw_option);
                    } else {
                        console_command cmd;
                        cmd.name = raw_option;
                        _result.emplace_back(cmd);
                    }
                }
            }
        }
        return _result;
    }
}
}