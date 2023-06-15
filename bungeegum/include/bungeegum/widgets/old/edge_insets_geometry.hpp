#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct edge_insets {
        edge_insets& bottom(const double size);
        edge_insets& bottom_left(const double size);
        edge_insets& bottom_right(const double size);
    };

}
}