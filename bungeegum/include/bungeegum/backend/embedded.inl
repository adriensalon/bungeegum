#pragma once

#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/process.fwd>

namespace bungeegum {

void bungeegum_functor::operator()()
{
    std::chrono::microseconds _max_fps_period_microseconds = std::chrono::microseconds(static_cast<unsigned int>(
        std::floorf(1000000.f / 60.f /* MAX FPS !!!*/)));
    std::chrono::microseconds _delta_time = _data.stopwatch.lap_at_least(_max_fps_period_microseconds);
    std::chrono::milliseconds _delta_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_max_fps_period_microseconds);
    // if (detail::update(_delta_milliseconds))
    //     detail::draw(true);
}

template <typename widget_t>
bungeegum_functor embed(widget_t& widget)
{
    bungeegum_functor _functor;
    // detail::protect_library([&]() {
    detail::global_widgets_manager.root = detail::global_widgets_manager.get(widget);
    detail::setup_overlay();
    // });

    protect_library([&]() {
        overlay::install_font();
        context.widgets.declare_root(widget);
    });
    // return [this]() {
    //     protect_library([&]() {
    //         std::chrono::milliseconds _delta_time = _functor.stopwatch.lap();
    //         float _delta_milliseconds = static_cast<float>(_delta_time.count());
    //         if (!has_userspace_thrown())
    //             context.update(_delta_milliseconds);
    //         context.draw();
    //     });
    // }
    return _functor;
}
}
