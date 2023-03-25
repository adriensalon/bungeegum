#pragma once

#include <bungeegum/core/context.fwd>
#include <bungeegum/core/exceptions.hpp>

namespace bungeegum {

template <typename widget_t>
std::function<void()> embed(widget_t& widget)
{
    protect_library([&]() {
        overlay::install_font();
        context.widgets.declare_root(widget);
    });
    return [this]() {
        protect_library([&]() {
            std::chrono::milliseconds _delta_time = _stopwatch.lap();
            float _delta_milliseconds = static_cast<float>(_delta_time.count());
            if (!has_userspace_thrown())
                context.tick(_delta_milliseconds);
            context.draw();
        });
    }
}

}
