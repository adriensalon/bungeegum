#include <bungeegum/core/global.fwd>
#include <bungeegum/core/pipeline.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/console.hpp>

namespace bungeegum {
namespace detail {
    namespace {

        void console_log_exception(const backtraced_exception& exception, const log_color color)
        {
            log("\"" + std::string(exception.what()) + "\"", log_color::black_or_white);
            log(" occured", color);
#if BUNGEEGUM_USE_BACKTRACE
            if (exception.tracing.empty()) {
                log(". \n", color);
            } else {
                log(" with trace : \n", color);
                for (const backtraced_step& _result : exception.tracing) {
                    std::string _trace_location = "[" + _result.file.filename().generic_string();
                    _trace_location += ", Ln " + std::to_string(_result.line);
                    _trace_location += ", Col " + std::to_string(_result.column) + "] ";
                    log(_trace_location, log_color::black_or_white);
                    log(_result.function + "\n", log_color::black_or_white);
                }
            }
#else
            log(". \n", color);
#endif
        }

        void console_log_error(const backtraced_exception& exception)
        {
            log("Error ", log_color::red);
            console_log_exception(exception, log_color::red);
        }

        void console_log_warning(const backtraced_exception& exception)
        {
            log("Warning ", log_color::yellow);
            console_log_exception(exception, log_color::yellow);
        }

        void console_log_message(const backtraced_exception& exception)
        {
            log("Message ", log_color::blue);
            console_log_exception(exception, log_color::blue);
        }

    }

    //     void protect_library(const std::function<void()>& try_callback)
    //     {
    // 		protect(try_callback, [] (const std::string& _what) {
    //        	 	log("GALERE C UNE ERREUR DANS MON CODE qui nest pas backtracee", log_color::red);
    // #if TOOLCHAIN_PLATFORM_EMSCRIPTEN

    // #else
    // 			std::terminate();
    // #endif
    // 		});
    //     }

    //     void protect_userspace(std::vector<backtraced_exception>& container, const std::function<void()>& try_callback)
    //     {
    // 		protect(try_callback, [&container] (const std::string& _what) {
    // 			// uncaught error in widget ... with message ... Please use the log_error() function to detect misconfiguration etc
    // 			backtraced_exception _exception(_what, 0, 0);
    // #if BUNGEEGUM_USE_OVERLAY
    //             container.push_back(_exception);
    // #else
    //             console_log_error(_exception);
    //             (void)container;
    // #endif
    // 		});
    //     }
}

void log_error(const std::string& what, const bool must_throw)
{
    detail::swapped_manager_data& _swapped = detail::get_swapped_global();
    detail::backtraced_exception _exception(what, 1u);
    if (_swapped.current == 0u) {
        detail::console_log_error(_exception);
    } else {
#if BUNGEEGUM_USE_OVERLAY
        _swapped.logs.userspace_errors.push_back(detail::log_data {
            1u,
            "tag",
            _exception.what(),
            "_swapped.widgets.current",
            _exception.tracing });
#else
        detail::console_log_error(_exception);
#endif
    }
    if (must_throw) {
        throw _exception;
    }
}

void log_error(const std::wstring& what, const bool must_throw)
{
    detail::swapped_manager_data& _swapped = detail::get_swapped_global();
    detail::backtraced_exception _exception(what, 1u);
    if (_swapped.current == 0u) {
        detail::console_log_error(_exception);
    } else {
#if BUNGEEGUM_USE_OVERLAY
        _swapped.logs.userspace_errors.push_back(detail::log_data {
            1u,
            "tag",
            _exception.what(),
            "_swapped.widgets.current",
            _exception.tracing });
#else
        detail::console_log_error(_exception);
#endif
    }
    if (must_throw) {
        throw _exception;
    }
}

void log_warning(const std::string& what)
{
    detail::swapped_manager_data& _swapped = detail::get_swapped_global();
    detail::backtraced_exception _exception(what, 1u);
    if (_swapped.current == 0u) {
        detail::console_log_warning(_exception);
    } else {
#if BUNGEEGUM_USE_OVERLAY
        _swapped.logs.userspace_warnings.push_back(detail::log_data {
            1u,
            "tag",
            _exception.what(),
            "_swapped.widgets.current",
            _exception.tracing });
#else
        detail::console_log_warning(_exception);
#endif
    }
}

void log_warning(const std::wstring& what)
{
    detail::swapped_manager_data& _swapped = detail::get_swapped_global();
    detail::backtraced_exception _exception(what, 1u);
    if (_swapped.current == 0u) {
        detail::console_log_warning(_exception);
    } else {
#if BUNGEEGUM_USE_OVERLAY
        _swapped.logs.userspace_warnings.push_back(detail::log_data {
            1u,
            "tag",
            _exception.what(),
            "_swapped.widgets.current",
            _exception.tracing });
#else
        detail::console_log_warning(_exception);
#endif
    }
}

void log_message(const std::string& what)
{
    detail::swapped_manager_data& _swapped = detail::get_swapped_global();
    detail::backtraced_exception _exception(what, 1u);
    if (_swapped.current == 0u) {
        detail::console_log_message(_exception);
    } else {
#if BUNGEEGUM_USE_OVERLAY
        _swapped.logs.userspace_messages.push_back(detail::log_data {
            1u,
            "tag",
            _exception.what(),
            "_swapped.widgets.current",
            _exception.tracing });
#else
        detail::console_log_message(_exception);
#endif
    }
}

void log_message(const std::wstring& what)
{
    detail::swapped_manager_data& _swapped = detail::get_swapped_global();
    detail::backtraced_exception _exception(what, 1u);
    if (_swapped.current == 0u) {
        detail::console_log_message(_exception);
    } else {
#if BUNGEEGUM_USE_OVERLAY
        _swapped.logs.userspace_messages.push_back(detail::log_data {
            1u,
            "tag",
            _exception.what(),
            "_swapped.widgets.current",
            _exception.tracing });
#else
        detail::console_log_message(_exception);
#endif
    }
}
}
