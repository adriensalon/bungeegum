//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/staticgui.hpp>

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
    template <typename... children_widgets_t>
    struct column : base_widget {
        column(const children_widgets_t&... children)
        {
            build_advanced(this, [this](build_advanced_context& context) {
                std::cout << "column\n";
            });
        }
    };

}
}