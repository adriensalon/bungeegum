#include <bungeegum/core/global.fwd>
#include <bungeegum/core/pipeline.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/console.hpp>

namespace bungeegum {
namespace detail {
    namespace {

        void dispatch_overlay(std::optional<std::reference_wrapper<log_data_map>>& map, backtraced_exception&& exception)
        {
#if BUNGEEGUM_USE_OVERLAY
            if (map.has_value()) {
                log_data_map& _map = map.value().get();
                std::string _key = exception.key();
                if (_map.find(_key) != _map.end()) {
                    log_data& _log = _map.at(_key);
                    if (_log.count < SIZE_MAX - 1) {
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
            log_error(exception);
            dispatch_overlay(_swapped.errors, std::move(exception));
        }

        void dispatch_warning(backtraced_exception&& exception)
        {
            swapped_manager_data& _swapped = get_swapped_global();
            log_warning(exception);
            dispatch_overlay(_swapped.warnings, std::move(exception));
        }

        void dispatch_message(backtraced_exception&& exception)
        {
            swapped_manager_data& _swapped = get_swapped_global();
            log_message(exception);
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

void error(const std::string& tag, const std::string& what, const bool must_throw)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    if (must_throw) {
        throw _exception;
    } else {
        detail::dispatch_error(std::move(_exception));
    }
}

void error(const std::string& tag, const std::wstring& what, const bool must_throw)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    if (must_throw) {
        throw _exception;
    } else {
        detail::dispatch_error(std::move(_exception));
    }
}

void warning(const std::string& tag, const std::string& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_warning(std::move(_exception));
}

void warning(const std::string& tag, const std::wstring& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_warning(std::move(_exception));
}

void message(const std::string& tag, const std::string& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_message(std::move(_exception));
}

void message(const std::string& tag, const std::wstring& what)
{
    detail::backtraced_exception _exception(tag, what, 1u);
    detail::dispatch_message(std::move(_exception));
}
}
