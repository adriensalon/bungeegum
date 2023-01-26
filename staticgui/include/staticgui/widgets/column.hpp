//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <variant>

#include <staticgui/staticgui.hpp>

template <size_t start_t, size_t end_t, size_t increment_t, typename function_t>
inline constexpr void static_for(function_t&& f)
{
    if constexpr (start_t < end_t) {
        f(std::integral_constant<decltype(start_t), start_t>());
        static_for<start_t + increment_t, end_t, increment_t>(f);
    }
}

namespace staticgui {
namespace widgets {

    /// @brief A widget that displays its children in a vertical array.
    /// @details To cause a child to expand to fill the available vertical space, wrap the child in
    /// an 'expanded' widget. The 'column' widget does not scroll (and in general it is considered an
    /// error to have more children in a 'column' than will fit in the available room). If you have a
    /// line of widgets and want them to be able to scroll if there is insufficient room, consider
    /// using a 'list_view'. For a horizontal variant see 'row'. If you only have one child, then
    /// consider using 'align' or 'center' to position the child.
    /// @tparam ...children_widgets_t
    struct column_widget {
        constexpr static char* internal_name = "column";
        STATICGUI_WIDGET(column_widget)

        template <typename... children_widgets_t>
        column_widget(children_widgets_t&... children)
        {
            std::tuple<children_widgets_t...> _tuple_children(std::forward<children_widgets_t>(children)...);
            constexpr size_t children_count = std::variant_size_v<std::variant<children_widgets_t...>>;
            static_for<0, children_count, 1>([&](auto index) {
                using child_type_t = std::variant_alternative_t<index, std::variant<children_widgets_t...>>;
                auto& _child = std::get<index>(_tuple_children);
                build(this, _child);
            });
            // build_advanced(this, [this](build_advanced_context& context) {
            //     std::cout << "column\n";
            // });
        }
    };

}
}

#define column(...) staticgui::make<staticgui::widgets::column_widget>(__VA_ARGS__)