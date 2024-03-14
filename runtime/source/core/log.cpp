#include <bungeegum/core/global.fwd>
#include <bungeegum/core/pipeline.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/console.hpp>

namespace bungeegum {
namespace detail {
    namespace {

        void dispatch_log(const backtraced_exception& exception, const log_color color)
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

        void dispatch_overlay(std::optional<std::reference_wrapper<log_data_map>>& map, backtraced_exception&& exception)
        {
#if BUNGEEGUM_USE_OVERLAY
            if (map.has_value()) {
                log_data_map& _map = map.value().get();
                std::string _key = exception.key();
                if (_map.find(_key) != _map.end()) {
                    log_data& _log = _map.at(_key);
                    if (_log.count < 999) { // harcoded go config
                        _map.at(_key).count++;
                    }
                } else {
                    _map.insert({ _key, std::move(exception) });
                }
            }
#endif
        }

        void dispatch_error(backtraced_exception&& exception)
        {
            swapped_manager_data& _swapped = get_swapped_global();
            log("Error ", log_color::red);
            dispatch_log(exception, log_color::red);
            dispatch_overlay(_swapped.errors, std::move(exception));
        }

        void dispatch_warning(backtraced_exception&& exception)
        {
            swapped_manager_data& _swapped = get_swapped_global();
            log("Warning ", log_color::yellow);
            dispatch_log(exception, log_color::yellow);
            dispatch_overlay(_swapped.warnings, std::move(exception));
        }

        void dispatch_message(backtraced_exception&& exception)
        {
            swapped_manager_data& _swapped = get_swapped_global();
            log("Message ", log_color::blue);
            dispatch_log(exception, log_color::blue);
            dispatch_overlay(_swapped.messages, std::move(exception));
        }

    }
    
    log_data::log_data(backtraced_exception&& caught_exception)
        : exception(std::move(caught_exception))
        , count(1u)
    {
    }

    void protect_dispatch(const std::function<void()>& try_callback)
    {
        protect(try_callback, [](backtraced_exception&& exception) {
            dispatch_error(std::move(exception));
        });
    }
}

void log_error(const std::string& tag, const std::string& what, const bool must_throw)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    if (must_throw) {
        throw _exception;
    } else {
        detail::dispatch_error(std::move(_exception));
    }
}

void log_error(const std::string& tag, const std::wstring& what, const bool must_throw)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    if (must_throw) {
        throw _exception;
    } else {
        detail::dispatch_error(std::move(_exception));
    }
}

void log_warning(const std::string& tag, const std::string& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_warning(std::move(_exception));
}

void log_warning(const std::string& tag, const std::wstring& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_warning(std::move(_exception));
}

void log_message(const std::string& tag, const std::string& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_message(std::move(_exception));
}

void log_message(const std::string& tag, const std::wstring& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_message(std::move(_exception));
}
}
