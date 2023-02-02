//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {
namespace detail {

    template <typename widget_t>
    void platform_state::launch(widget_t& widget)
    {
        _context.make_at_root(widget);

        _window = std::make_unique<glue::window>();
        _window->run([this]() {

        });

        // glue::launch_window(
        //     "staticgui v0.0",
        //     1600, 900,
        //     [this]() {

        //     },
        //     [this]() {

        //     },
        //     [this]() {

        //     });
    }

    template <typename widget_t>
    void platform_state::attach(widget_t& widget)
    {
    }

}
}
