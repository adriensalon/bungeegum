//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/context/exceptions.hpp>
#include <staticgui/overlay/overlay.hpp>


namespace staticgui {

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
