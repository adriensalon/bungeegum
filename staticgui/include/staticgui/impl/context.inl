//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <memory>
#include <tree.hh>
#include <unordered_map>

#include <staticgui/staticgui.hpp>
#include <staticgui/utils/ecs.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        struct runtime_widget_component {
            std::unique_ptr<std::type_info> typeinfo = nullptr;
            std::any untyped = nullptr;
            runtime_widget_data data;
        };

        struct context_impl {

            inline static ecs::registry events_registry;
            inline static ecs::registry curves_registry;
            inline static ecs::registry animations_registry;
            inline static ecs::registry styles_registry;
            inline static ecs::registry widgets_registry;

            inline static std::unordered_map<id::id_int, runtime_widget_component*> widgets_staging_map;
            inline static std::tree<runtime_widget_component*> widgets_tree;

            inline static context_impl& get_context_impl()
            {
                static context_impl _context_impl;
                return _context_impl;
            }
        };

        struct context_manager {

            inline static context& get_context()
            {
                static context _context;
                return _context;
            }
        };

    }
}

context::context()
    : _impl(internal::impl::context_impl::get_context_impl())
{
}

context& get_context()
{
    return internal::impl::context_manager::get_context();
}

}