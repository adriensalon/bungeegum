#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    struct Alignment {
        float2 value;

        Alignment& add(const Alignment& other);
    };

    struct Align {

        Align& alignment(const Alignment& value);

        template <typename child_widget_t>
        Align& child(child_widget_t& value)
        {
            adopt(this, value);
            return *this;
        }

        Align& heightFactor(const float value);

        Align& widthFactor(const float value);

        //
        //
        //

        Align interact(interact_command& command);
        Align resolve(resolve_command& command);
        Align draw(draw_command& command);
    };

}
}