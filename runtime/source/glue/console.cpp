#include <bungeegum/glue/toolchain.hpp>
#include <bungeegum/glue/console.hpp>

#include <iostream>
#include <type_traits>
#include <vector>

#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
#include <windows.h>
#endif

namespace bungeegum {
namespace detail {

    namespace {

        template <typename string_t>
        void console_print(const string_t& message, const log_color color)
        {
            static_assert(std::is_same_v<string_t, std::string> || std::is_same_v<string_t, std::wstring>, "String type must be one of : std::string, std::wstring.");
#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
            HANDLE _handle = GetStdHandle(STD_OUTPUT_HANDLE);
            int _color = 7;
            if (color == log_color::blue)
                _color = 1;
            else if (color == log_color::green)
                _color = 2;
            else if (color == log_color::cyan)
                _color = 3;
            else if (color == log_color::red)
                _color = 4;
            else if (color == log_color::magenta)
                _color = 5;
            else if (color == log_color::yellow)
                _color = 6;
            SetConsoleTextAttribute(_handle, static_cast<WORD>(_color));
#elif TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_MACOS
            std::string _color = "\033[0m";
            if (color == log_color::blue)
                _color = "\033[0;34m";
            else if (color == log_color::green)
                _color = "\033[0;32m";
            else if (color == log_color::cyan)
                _color = "\033[0;36m";
            else if (color == log_color::red)
                _color = "\033[0;31m";
            else if (color == log_color::magenta)
                _color = "\033[0;35m";
            else if (color == log_color::yellow)
                _color = "\033[0;33m";
            std::cout << _color;
#endif
            if constexpr (std::is_same_v<string_t, std::string>) {
                std::cout << message;
            } else if constexpr (std::is_same_v<string_t, std::wstring>) {
                std::wcout << message;
            }
#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
            SetConsoleTextAttribute(_handle, 7);
#elif TOOLCHAIN_PLATFORM_LINUX || TOOLCHAIN_PLATFORM_MACOS
            std::cout << "\033[0m";
#endif
        }
    }

    redirect_guard::redirect_guard(std::streambuf* buffer)
        : _captured_cout(std::cout.rdbuf(buffer))
    {
    }

    redirect_guard::~redirect_guard()
    {
        std::cout.rdbuf(_captured_cout);
    }

    redirect_wide_guard::redirect_wide_guard(std::wstreambuf* buffer)
        : _captured_wcout(std::wcout.rdbuf(buffer))
    {
    }

    redirect_wide_guard::~redirect_wide_guard()
    {
        std::wcout.rdbuf(_captured_wcout);
    }

    void log(const std::string& message, const log_color color)
    {
        console_print<std::string>(message, color);
    }

    void log(const std::wstring& message, const log_color color)
    {
        console_print<std::wstring>(message, color);
    }
}
}