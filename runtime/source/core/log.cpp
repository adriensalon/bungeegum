#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/console.hpp>

namespace bungeegum {
namespace detail {
    namespace {

        void console_log_exception(const backtraced_exception& exception, const console_color color)
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
            console_log(". \n", color);
#endif
        }		

		void console_log_error(const backtraced_exception& exception)
		{
			console_log("Error ", console_color::red);
			console_log_exception(exception, console_color::red);
		}

		void console_log_warning(const backtraced_exception& exception)
		{
			console_log("Warning ", console_color::yellow);
			console_log_exception(exception, console_color::yellow);
		}

		void console_log_message(const backtraced_exception& exception)
		{
			console_log("Message ", console_color::blue);
			console_log_exception(exception, console_color::blue);
		}
    }

//     void protect_library(const std::function<void()>& try_callback)
//     {
// 		protect(try_callback, [] (const std::string& _what) {
//        	 	console_log("GALERE C UNE ERREUR DANS MON CODE qui nest pas backtracee", console_color::red);
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
    detail::backtraced_exception _exception(what, 1u);	
#if BUNGEEGUM_USE_OVERLAY
    detail::global().logs.userspace_errors.push_back(_exception);
#else
    detail::console_log_error(_exception);
#endif
	if (must_throw) {
		throw _exception;
	}
}

void log_error(const std::wstring& what, const bool must_throw)
{
	detail::backtraced_exception _exception(what, 1u);	
#if BUNGEEGUM_USE_OVERLAY
    detail::global().logs.userspace_errors.push_back(_exception);
#else
    detail::console_log_error(_exception);
#endif
	if (must_throw) {
		throw _exception;
	}
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
