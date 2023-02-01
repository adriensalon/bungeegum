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
#include <tree.hh>
#include <typeindex>
#include <unordered_map>

// #include <staticgui/glue/id_integer.hpp>
// #include <staticgui/glue/registry.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        struct widget_data {
            glue::id_integer this_id = glue::id_generator::create();
            std::vector<glue::id_integer> children_ids;
            std::function<void(layout&)> paint_callback = nullptr;
        };

        struct widget_impl {
            std::unique_ptr<std::type_index> typeindex = nullptr;
            std::any untyped = nullptr;
            // ecs::entity owner;
            widget_data data;
        };

        inline static glue::registry widgets_container;
        inline static std::unordered_map<glue::id_integer, std::reference_wrapper<widget_impl>> widgets_refs_staging_container;
        inline static std::tree<widget_impl*> widgets_ptrs_container;
        // inline static glue::tree<std::reference_wrapper<widget_impl>> widgets_refs_container;
    }
}
}
