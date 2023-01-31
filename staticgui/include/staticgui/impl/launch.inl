//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <tree.hh>
template <typename value_t>
void print_tree(const std::tree<value_t>& tree)
{
    std::tree<value_t>::iterator _it = tree.begin();
    _it++;
    std::cout << "application" << std::endl;
    while (_it != tree.end()) {

        int _depth = tree.depth(_it);
        for (int _k = 0; _k < _depth; _k++)
            std::cout << "   ";
        std::cout << "|__ ";
        std::cout << (*_it)->typeindex->name();
        if ((*_it)->data.paint_callback)
            std::cout << " [painter]";
        std::cout << std::endl;
        _it++;
    }
}

namespace staticgui {

template <typename widget_t>
void launch(widget_t& widget)
{
    using namespace internal::impl;
    struct application_launcher {
        widget_data internal_data;
    };
    application_launcher _launcher;
    build(&_launcher, widget); // besoin de la struct degueu pour ca ?
    widgets_ptrs_container.insert(widgets_ptrs_container.begin(), {});

    std::function<void(std::tree<widget_impl*>::iterator, const glue::id_integer)> _emplace =
        [&](std::tree<widget_impl*>::iterator parent_it, const glue::id_integer parent_id) {
            widget_impl& _runtime_parent_widget = widgets_refs_staging_container.at(parent_id);
            parent_it = widgets_ptrs_container.append_child(parent_it, &_runtime_parent_widget); // move pcq on erase juste apres
            widgets_refs_staging_container.erase(parent_id);
            for (auto& _child_id : (*parent_it)->data.children_ids)
                _emplace(parent_it, _child_id);
        };
    _emplace(widgets_ptrs_container.begin(), _launcher.internal_data.children_ids[0]);

    while (true) {
        print_tree<widget_impl*>(widgets_ptrs_container);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
}

}