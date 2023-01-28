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
    using namespace internal::impl;
    struct application_launcher {
        runtime_widget_data internal_data;
    };
    application_launcher _launcher;
    build(&_launcher, widget); // besoin de la struct degueu pour ca ?
    widgets_ptrs_container.insert(widgets_ptrs_container.begin(), {});

    std::function<void(std::tree<runtime_widget_component*>::iterator, const internal::id::integer)> _emplace =
        [&](std::tree<runtime_widget_component*>::iterator parent_it, const internal::id::integer parent_id) {
            runtime_widget_component* _runtime_parent_widget = widgets_ptrs_staging_container[parent_id];
            parent_it = widgets_ptrs_container.append_child(parent_it, _runtime_parent_widget);
            for (auto& _child_id : (*parent_it)->data.children_ids)
                _emplace(parent_it, _child_id);
        };
    _emplace(widgets_ptrs_container.begin(), _launcher.internal_data.children_ids[0]);

    while (true) {
        internal::id::print_tree<runtime_widget_component*>(widgets_ptrs_container);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

}