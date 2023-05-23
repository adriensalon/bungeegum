#pragma once

#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    template <typename widget_t>
    using resolve_function = decltype(std::declval<widget_t>().resolve(std::declval<resolve_command&>()));

    template <typename widget_t>
    constexpr bool has_resolve_function = is_detected_exact_v<void, resolve_function, widget_t>;

    template <typename widget_t>
    constexpr void detect_on_resolve(widget_t* widget)
    {
        if constexpr (detail::has_resolve_function<widget_t>) {
            detail::untyped_widget_data& _widget_data = detail::get_untyped_widget(*widget);
            _widget_data.resolver_command = resolve_command();
            _widget_data.resolver = [widget](resolve_command& command) {
                widget->resolve(command);
            };
        }
    }
}

template <typename widget_t>
void on_resolve(widget_t* widget, const std::function<void(resolve_command&)>& resolve_callback)
{
    if (!resolve_callback)
        detail::throw_error<error_type::bad_usage>("resolve callback is nullptr");
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get(*widget);
    if (!_widget_data.resolver_command.has_value())
        _widget_data.resolver_command = resolve_command();
    _widget_data.resolver = [widget](resolve_command& command) {
        resolve_callback(command);
    };
}

template <typename widget_t>
void must_resolve(widget_t* widget)
{
    if (detail::widgets_context.accessors.find(reinterpret_cast<void*>(widget)) == detail::widgets_context.accessors.end())
        detail::register_widget(&widget);
    detail::untyped_widget_data& _data = detail::get_untyped_widget(*widget);
    detail::widgets_context.resolvables.emplace_back(_data);
}

}