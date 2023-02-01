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

template <typename widget_t>
void launch(widget_t& widget)
{
    detail::global_widgets.build_root(widget);

    std::cout << "application" << std::endl;
    detail::global_widgets.iterate_datas([&](detail::widget_data& _widget_data) {
        unsigned int _depth = detail::global_widgets.get_depth(_widget_data);
        for (unsigned int _k = 0; _k < _depth; _k++)
            std::cout << "   ";
        std::cout << "|__ ";
        std::cout << _widget_data.kind->name();
        if (_widget_data.paint_callback)
            std::cout << " [painter]";
        std::cout << std::endl;
    });

    // while (true) {
    //     print_tree<widget_impl*>(widgets_ptrs_container);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // }
}

}