#pragma once

#include <bungeegum/core/exceptions.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {
// namespace detail {

// template <typename widget_t>
// using resolve_function = decltype(std::declval<widget_t>().resolve(std::declval<resolve_command&>()));

// template <typename widget_t>
// constexpr bool has_resolve_function = is_detected_exact_v<void, resolve_function, widget_t>;

// template <typename widget_t>
// constexpr void access::detect_on_resolve(widget_t& widget)
// {
//     if constexpr (detail::has_resolve_function<widget_t>) {
//         detail::untyped_widget_data& _widget_data = detail::get_untyped_widget(widget);
//         _widget_data.resolver_command = resolve_command();
//         _widget_data.resolver = [&widget](resolve_command& command) {
//             widget.resolve(command);
//         };
//     }
//     }
// }

template <typename value_t>
std::optional<value_t>& resolve_command::properties(const std::string& name)
{
    static std::unordered_map<std::uintptr_t, std::unordered_map<std::string, std::optional<value_t>>> _properties = {};
    std::uintptr_t _raw_command = detail::raw_cast<resolve_command>(this);
    return _properties[_raw_command][name];
}

template <typename child_widget_t>
float2 resolve_command::resolve_child(const typed_widget_t<child_widget_t>& child_widget, const float2 min_size, const float2 max_size)
{
    // todo
    (void)child_widget;
    (void)min_size;
    (void)max_size;
    return zero<float2>;
}

template <typename child_widget_t>
void resolve_command::position_child(typed_widget_t<child_widget_t>& child_widget, const float2 position)
{
    // todo
    (void)child_widget;
    (void)position;
}

template <typename widget_t>
void on_resolve(widget_t& widget, const std::function<void(resolve_command&)>& resolve_callback)
{
    if (!resolve_callback)
        detail::throw_error<error_type::bad_usage>("resolve callback is nullptr");
    detail::untyped_widget_data& _widget_data = detail::widgets_context.get(widget);
    if (!_widget_data.resolver_command.has_value())
        _widget_data.resolver_command = resolve_command();
    _widget_data.resolver = [](resolve_command& command) {
        resolve_callback(command);
    };
}

template <typename widget_t>
void on_resolve(widget_t* widget, const std::function<void(resolve_command&)>& resolve_callback)
{
    on_resolve<widget_t>(widget, resolve_callback);
}

template <typename widget_t>
void must_resolve(widget_t& widget)
{
    std::uintptr_t _raw_widget = detail::get_raw_widget<widget_t>(widget);
    if (!detail::is_widget_registered(_raw_widget))
        detail::register_widget(widget, _raw_widget);
    detail::untyped_widget_data& _data = detail::get_untyped_widget(widget);
    detail::widgets_context.resolvables.emplace_back(_data);
}

template <typename widget_t>
void must_resolve(widget_t* widget)
{
    must_resolve(*widget);
}
}