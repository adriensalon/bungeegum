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
        if constexpr (is_platform_win32 || is_platform_uwp) {
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
        } else if constexpr (is_platform_linux || is_platform_macos) {
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
        if constexpr (is_platform_win32 || is_platform_uwp)
            SetConsoleTextAttribute(_handle, 7);
        else if constexpr (is_platform_linux || is_platform_macos)
            std::cout << "\033[0m";
    }
}
}