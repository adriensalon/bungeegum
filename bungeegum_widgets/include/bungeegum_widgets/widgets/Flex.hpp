#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Axis.hpp>
#include <bungeegum_widgets/core/CrossAxisAlignment.hpp>
#include <bungeegum_widgets/core/MainAxisAlignment.hpp>
#include <bungeegum_widgets/core/MainAxisSize.hpp>
#include <bungeegum_widgets/core/VerticalDirection.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that displays its children in a one-dimensional array.
    /// @details The Flex widget allows you to control the axis along which the children are placed
    /// (horizontal or vertical). This is referred to as the main axis. If you know the main axis
    /// in advance, then consider using a Row (if it's horizontal) or Column (if it's vertical)
    /// instead, because that will be less verbose.
    /// To cause a child to expand to fill the available space in the direction of this widget's
    /// main axis, wrap the child in an Expanded widget.
    /// The Flex widget does not scroll (and in general it is considered an error to have more
    /// children in a Flex than will fit in the available room). If you have some widgets and want
    /// them to be able to scroll if there is insufficient room, consider using a ListView.
    /// The Flex widget does not allow its children to wrap across multiple horizontal or vertical
    /// runs. For a widget that allows its children to wrap, consider using the Wrap widget instead
    /// of Flex.
    struct Flex {

        template <typename... children_widgets_t>
        Flex& children(children_widgets_t&... values)
        {
            // veritable unmake en vrai ici
            _children.clear();
            (_children.push_back(adopt(this, values)), ...);
            return *this;
        }

        // clip behavior

        Flex& crossAxisAlignment(const CrossAxisAlignment value);

        Flex& direction(const Axis value);

        Flex& mainAxisAlignment(const MainAxisAlignment value);

        Flex& mainAxisSize(const MainAxisSize value);

        // text baseline

        // text direction

        Flex& verticalDirection(const VerticalDirection value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::vector<runtime_widget> _children = {};
        CrossAxisAlignment _crossAxisAlignment = CrossAxisAlignment::center;
        Axis _direction = Axis::horizontal;
        MainAxisAlignment _mainAxisAlignment = MainAxisAlignment::start; // ? ig
        MainAxisSize _mainAxisSize = MainAxisSize::min; // ? ig
        VerticalDirection _verticalDirection = VerticalDirection::down; // ? ig
    };

}
}