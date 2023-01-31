//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>
#include <memory>
#include <typeindex>

#include <staticgui/glue/id_integer.hpp>

namespace staticgui {
namespace detail {

    struct widget_data {
        glue::id_integer this_id = glue::id_generator::create();
        std::vector<glue::id_integer> children_ids;
        std::function<void(layout&)> paint_callback = nullptr;
    };

    struct widget_component {
        std::unique_ptr<std::type_index> typeindex = nullptr;
        std::any untyped = nullptr;
        widget_data data;
        glue::id_integer entity;
    };

}
}