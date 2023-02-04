#if defined(_WIN32)
#include <windows.h>
#endif

#include <iostream>

#include <staticgui/glue/console.hpp>

namespace staticgui {
namespace glue {

    void console_log(const std::string& message, const console_color color, const bool newline)
    {
#if defined(__EMSCRIPTEN__)
#elif defined(_WIN32)
        HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
        int col = 7;
        if (color == console_color::blue)
            col = 1;
        else if (color == console_color::green)
            col = 2;
        else if (color == console_color::cyan)
            col = 3;
        else if (color == console_color::red)
            col = 4;
        else if (color == console_color::magenta)
            col = 5;
        else if (color == console_color::yellow)
            col = 6;
        SetConsoleTextAttribute(h_console, col);
#elif defined(__gnu_linux__)
        std::string col = "\033[0m";
        if (color == console_color::blue)
            col = "\033[0;34m";
        else if (color == console_color::green)
            col = "\033[0;32m";
        else if (color == console_color::cyan)
            col = "\033[0;36m";
        else if (color == console_color::red)
            col = "\033[0;31m";
        else if (color == console_color::magenta)
            col = "\033[0;35m";
        else if (color == console_color::yellow)
            col = "\033[0;33m";
        std::cout << col;
#endif
        std::cout << message;
#if defined(__EMSCRIPTEN__)
#elif defined(_WIN32)
        SetConsoleTextAttribute(h_console, 7);
#elif defined(__gnu_linux__)
        std::cout << "\033[0m";
#endif
        if (newline)
            std::cout << std::endl;
    }

#if !defined(__EMSCRIPTEN__)

    std::vector<console_command> console_args(int argc, char* argv[])
    {
        auto result = std::vector<console_command>();
        auto raw_options = std::vector<std::string>(argv + 1, argv + argc);
        for (auto& raw_option : raw_options) {
            if (!raw_option.empty()) {
                if (raw_option[0] != '-') {
                    if (result.empty())
                        return {};
                    result.back().args.emplace_back(raw_option);
                } else {
                    console_command cmd;
                    cmd.name = raw_option;
                    result.emplace_back(cmd);
                }
            }
        }
        return result;
    }

#endif
}

}