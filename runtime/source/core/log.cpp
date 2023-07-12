#pragma once

#include <optional>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/console.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        void log_error_to_console(backtraced_exception&& exception)
        {
            console_log("Fatal error ", console_color::yellow);
            console_log(L"[" + exception.wide_what() + L"]", console_color::default);
            console_log(" occured outside userspace with trace : \n", console_color::yellow);
            for (const backtraced_result& _result : exception.tracing) {
                std::string _trace_location = "[" + _result.primary.file.generic_string();
                _trace_location += ", Ln " + std::to_string(_result.primary.line);
                _trace_location += ", Col " + std::to_string(_result.primary.column) + "]";
                console_log(_trace_location, console_color::default);
                console_log(_result.primary.function + "\n", console_color::default);
            }
        }

    }

    void logs_manager::protect_library(const std::function<void()>& try_callback)
    {
        try {
            try_callback();

        } catch (detail::backtraced_exception&& _exception) {
            log_error_to_console(std::move(_exception));

        } catch (const char* _what) {
            log_error_to_console(detail::backtraced_exception(std::string(_what)));

        } catch (const std::string& _what) {
            log_error_to_console(detail::backtraced_exception(_what));

        } catch (const std::wstring& _what) {
            log_error_to_console(detail::backtraced_exception(_what));

        } catch (std::exception& _exception) {
            log_error_to_console(detail::backtraced_exception(_exception.what()));

        } catch (...) {
            log_error_to_console(detail::backtraced_exception("Unknown exception occured."));
        }
    }

    void logs_manager::protect_userspace(const std::function<void()>& try_callback)
    {
        try {
            try_callback();

        } catch (detail::backtraced_exception&& _exception) {
            detail::global_manager::logs().userspace_errors.push_back(std::move(_exception));

        } catch (const char* _what) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception(std::string(_what)));

        } catch (const std::string& _what) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception(_what));

        } catch (const std::wstring& _what) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception(_what));

        } catch (std::exception& _exception) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception(_exception.what()));

        } catch (...) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception("Unknown exception occured."));
        }
    }
}

void log_error(const std::string& what)
{
    throw detail::backtraced_exception(what);
}

void log_warning(const std::string& what)
{
    detail::global_manager::logs().userspace_warnings.push_back(what);
}

void log_message(const std::string& what)
{
    detail::global_manager::logs().userspace_messages.push_back(what);
}

}
