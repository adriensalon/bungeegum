#pragma once

#include <iostream>
#include <sstream>

namespace bungeegum {
namespace detail {

    /// @brief
    /// @param wstr
    std::wstring widen(const std::string& str);

    /// @brief
    /// @param wstr
    std::string narrow(const std::wstring& wstr);

    /// @brief
    enum struct console_color {
        black_or_white,
        blue,
        green,
        cyan,
        red,
        magenta,
        yellow
    };

    /// @brief
    void console_log(const std::string& message, const console_color color = console_color::black_or_white);

    /// @brief
    void console_log(const std::wstring& message, const console_color color = console_color::black_or_white);

    /// @brief
    /// @details Instances of this struct can not be copied or moved.
    struct console_redirect {
        console_redirect() = delete;
        console_redirect(const console_redirect& other) = delete;
        console_redirect& operator=(const console_redirect& other) = delete;
        console_redirect(console_redirect&& other) = delete;
        console_redirect& operator=(console_redirect&& other) = delete;
        ~console_redirect();

        /// @brief
        console_redirect(std::streambuf* buffer);

    private:
        std::streambuf* _captured_cout;
    };

    /// @brief
    /// @details Instances of this struct can not be copied or moved.
    struct console_redirect_wide {
        console_redirect_wide() = delete;
        console_redirect_wide(const console_redirect_wide& other) = delete;
        console_redirect_wide& operator=(const console_redirect_wide& other) = delete;
        console_redirect_wide(console_redirect_wide&& other) = delete;
        console_redirect_wide& operator=(console_redirect_wide&& other) = delete;
        ~console_redirect_wide();

        /// @brief
        console_redirect_wide(std::wstreambuf* buffer);

    private:
        std::wstreambuf* _captured_wcout;
    };
}
}
