#pragma once

#include <optional>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/console.hpp>

namespace bungeegum {
namespace detail {

    void logs_manager::protect_library(const std::function<void()>& try_callback)
    {
        // try_catch_nullptr(
        //     [&]() {
        try {
            try_callback();
            // } catch (library_bad_implementation_exception& _exception) {
            //     // std::cout << "BAD IMPL 2 \n";
            //     // bad implementation exception (traced)
            //     for (auto& _trace : _exception.tracing)
            //         console_log(_trace.primary.function, console_color::green);
            // } catch (const std::exception& _exception) {
            //     // std::cout << "UNKNOWN 2\n";
            //     // std::cout << _exception.what() << std::endl;
            //     (void)_exception;
            // bad implementation exception (non traced)
        } catch (...) {
        }
    }

    void logs_manager::protect_userspace(const std::function<void()>& try_callback)
    {
        try {
            try_callback();

        } catch (detail::backtraced_exception&& _exception) {
            detail::global_manager::logs().userspace_errors.push_back(std::move(_exception));

        } catch (std::exception& _exception) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception(_exception.what()));

        } catch (const std::string& _what) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception(_what));

        } catch (const char* _what) {
            detail::global_manager::logs().userspace_errors.push_back(detail::backtraced_exception(std::string(_what)));

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
