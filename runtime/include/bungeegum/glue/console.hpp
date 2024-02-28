#pragma once

#include <iostream>
#include <functional>
#include <sstream>

namespace bungeegum {
namespace detail {

    /// @brief
    enum struct log_color {
        black_or_white,
        blue,
        green,
        cyan,
        red,
        magenta,
        yellow
    };

    /// @brief
    void log(const std::string& message, const log_color color = log_color::black_or_white);

    /// @brief
    void log(const std::wstring& message, const log_color color = log_color::black_or_white);

    /// @brief
    /// @details Instances of this struct can not be copied or moved
    struct redirect_guard {
        redirect_guard() = delete;
        redirect_guard(const redirect_guard& other) = delete;
        redirect_guard& operator=(const redirect_guard& other) = delete;
        redirect_guard(redirect_guard&& other) = delete;
        redirect_guard& operator=(redirect_guard&& other) = delete;
        ~redirect_guard();

        /// @brief
        redirect_guard(std::streambuf* buffer);

    private:
        std::streambuf* _captured_cout;
    };

    /// @brief
    /// @details Instances of this struct can not be copied or moved
    struct redirect_wide_guard {
        redirect_wide_guard() = delete;
        redirect_wide_guard(const redirect_wide_guard& other) = delete;
        redirect_wide_guard& operator=(const redirect_wide_guard& other) = delete;
        redirect_wide_guard(redirect_wide_guard&& other) = delete;
        redirect_wide_guard& operator=(redirect_wide_guard&& other) = delete;
        ~redirect_wide_guard();

        /// @brief
        redirect_wide_guard(std::wstreambuf* buffer);

    private:
        std::wstreambuf* _captured_wcout;
    };

    /// @brief 
    /// @param callback 
    std::string redirect(const std::function<void()>& callback);

    /// @brief 
    /// @param callback 
    std::wstring redirect_wide(const std::function<void()>& callback);
}
}
