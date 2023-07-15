#pragma once

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/toolchain.hpp>

#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
#include <Windows.h>
#endif

namespace bungeegum {
namespace detail {

    namespace {

        void protect_seh(const std::function<void()>& try_callback, const std::function<void()>& catch_callback)
        {
#if TOOLCHAIN_PLATFORM_WIN32 || TOOLCHAIN_PLATFORM_UWP
            __try {
                try_callback();
            } __except (EXCEPTION_EXECUTE_HANDLER) {
                catch_callback();
            }
#else
            try_callback();
            (void)catch_callback;
#endif
        }

        void console_log_exception(backtraced_exception&& exception, const console_color color)
        {
            console_log("\"" + std::string(exception.what()) + "\"", console_color::black_or_white);
            console_log(" occured", color);
#if BUNGEEGUM_USE_BACKTRACE
            if (exception.tracing.empty()) {
                console_log(". \n", color);
            } else {
                console_log(" with trace : \n", color);
                for (const backtraced_result& _result : exception.tracing) {
                    std::string _trace_location = "[" + _result.primary.file.filename().generic_string();
                    _trace_location += ", Ln " + std::to_string(_result.primary.line);
                    _trace_location += ", Col " + std::to_string(_result.primary.column) + "] ";
                    console_log(_trace_location, console_color::black_or_white);
                    console_log(_result.primary.function + "\n", console_color::black_or_white);
                }
            }

#else
            console_log(". \n", console_color::color);
#endif
        }

        void console_log_error_or_push_back(backtraced_exception&& exception, std::vector<backtraced_exception>& container)
        {
#if BUNGEEGUM_USE_OVERLAY
            container.push_back(std::move(exception));
#else
            console_log_error(std::move(exception));
            (void)container;
#endif
        }
    }

    void console_log_error(backtraced_exception&& exception)
    {
        console_log("Error ", console_color::red);
        console_log_exception(std::move(exception), console_color::red);
    }

    void console_log_warning(backtraced_exception&& exception)
    {
        console_log("Warning ", console_color::yellow);
        console_log_exception(std::move(exception), console_color::yellow);
    }

    void console_log_message(backtraced_exception&& exception)
    {
        console_log("Message ", console_color::blue);
        console_log_exception(std::move(exception), console_color::blue);
    }

    void logs_manager::protect_library(const std::function<void()>& try_callback)
    {
        protect_seh(
            [&try_callback]() {
                try {
                    try_callback();

                } catch (detail::backtraced_exception&& _exception) {
                    console_log_error(std::move(_exception));

                } catch (const char* _what) {
                    console_log_error(detail::backtraced_exception(std::string(_what, 0u, 0u)));

                } catch (const std::string& _what) {
                    console_log_error(detail::backtraced_exception(_what, 0u, 0u));

                } catch (const std::wstring& _what) {
                    console_log_error(detail::backtraced_exception(_what, 0u, 0u));

                } catch (std::exception& _exception) {
                    console_log_error(detail::backtraced_exception(_exception.what(), 0u, 0u));

                } catch (...) {
                    console_log_error(detail::backtraced_exception("Unknown exception occured.", 0u, 0u));
                }
            },
            []() {
                console_log_error(detail::backtraced_exception("Unknown Windows SEH occured.", 0u, 0u));
            });
    }

    void logs_manager::protect_userspace(const std::function<void()>& try_callback)
    {
        protect_seh(
            [this, &try_callback]() {
                try {
                    try_callback();

                } catch (detail::backtraced_exception&& _exception) {
                    console_log_error_or_push_back(std::move(_exception), userspace_errors);

                } catch (const char* _what) {
                    console_log_error_or_push_back(detail::backtraced_exception(std::string(_what), 0u, 0u), userspace_errors);

                } catch (const std::string& _what) {
                    console_log_error_or_push_back(detail::backtraced_exception(_what, 0u, 0u), userspace_errors);

                } catch (const std::wstring& _what) {
                    console_log_error_or_push_back(detail::backtraced_exception(_what, 0u, 0u), userspace_errors);

                } catch (std::exception& _exception) {
                    console_log_error_or_push_back(detail::backtraced_exception(_exception.what(), 0u, 0u), userspace_errors);

                } catch (...) {
                    console_log_error_or_push_back(detail::backtraced_exception("Unknown exception occured.", 0u, 0u), userspace_errors);
                }
            },
            [this]() {
                console_log_error_or_push_back(detail::backtraced_exception("Unknown Windows SEH occured.", 0u, 0u), userspace_errors);
            });
    }
}

void log_error(const std::string& what)
{
    throw detail::backtraced_exception(what, 1u);
}

void log_error(const std::wstring& what)
{
    throw detail::backtraced_exception(what, 1u);
}

void log_warning(const std::string& what)
{
#if BUNGEEGUM_USE_OVERLAY
    detail::global().logs.userspace_warnings.push_back(detail::backtraced_exception(what, 1u));
#else
    detail::console_log_warning(detail::backtraced_exception(what, 1u));
#endif
}

void log_warning(const std::wstring& what)
{
#if BUNGEEGUM_USE_OVERLAY
    detail::global().logs.userspace_warnings.push_back(detail::backtraced_exception(what, 1u));
#else
    detail::console_log_warning(detail::backtraced_exception(what, 1u));
#endif
}

void log_message(const std::string& what)
{
#if BUNGEEGUM_USE_OVERLAY
    detail::global().logs.userspace_messages.push_back(detail::backtraced_exception(what, 1u));
#else
    detail::console_log_message(detail::backtraced_exception(what, 1u));
#endif
}

void log_message(const std::wstring& what)
{
#if BUNGEEGUM_USE_OVERLAY
    detail::global().logs.userspace_messages.push_back(detail::backtraced_exception(what, 1u));
#else
    detail::console_log_message(detail::backtraced_exception(what, 1u));
#endif
}
}
