#include <iostream>
#include <locale>
#include <type_traits>
#include <vector>

#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/toolchain.hpp>

#if (TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP)
#include <windows.h>
#endif

namespace bungeegum {
namespace detail {

    namespace {

        template <typename string_t>
        void console_print(const string_t& message, const console_color color)
        {
            static_assert(std::is_same_v<string_t, std::string> || std::is_same_v<string_t, std::wstring>, "String type must be one of : std::string, std::wstring.");
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
            if constexpr (std::is_same_v<string_t, std::string>) {
                std::cout << message;
            } else if constexpr (std::is_same_v<string_t, std::wstring>) {
                std::wcout << message;
            }
            if constexpr (is_platform_win32 || is_platform_uwp)
                SetConsoleTextAttribute(_handle, 7);
            else if constexpr (is_platform_linux || is_platform_macos)
                std::cout << "\033[0m";
        }
    }

    std::wstring widen(const std::string& str)
    {
        std::vector<wchar_t> _buffer(str.size());
        std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(str.data(), str.data() + str.size(), _buffer.data());
        return std::wstring(_buffer.data(), _buffer.size());
    }

    std::string narrow(const std::wstring& wstr)
    {
        std::vector<char> _buffer(wstr.size());
        std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(wstr.data(), wstr.data() + wstr.size(), '0', _buffer.data());
        return std::string(_buffer.data(), _buffer.size());
    }

    console_redirect::console_redirect(std::streambuf* buffer)
        : _captured_cout(std::cout.rdbuf(buffer))
    {
    }

    console_redirect::~console_redirect()
    {
        std::cout.rdbuf(_captured_cout);
    }

    console_redirect_wide::console_redirect_wide(std::wstreambuf* buffer)
        : _captured_wcout(std::wcout.rdbuf(buffer))
    {
    }

    console_redirect_wide::~console_redirect_wide()
    {
        std::wcout.rdbuf(_captured_wcout);
    }

    void console_log(const std::string& message, const console_color color)
    {
        console_print<std::string>(message, color);
    }

    void console_log(const std::wstring& message, const console_color color)
    {
        console_print<std::wstring>(message, color);
    }
}
}