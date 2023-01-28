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
#include <typeinfo>
#include <unordered_map>

#include <staticgui/staticgui.hpp>
#include <staticgui/utils/ecs.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        struct runtime_event_component {
            std::vector<std::type_info> typeinfos;
            std::any untyped = nullptr;

            // template <typename... values_t>
            // std::function<void(values_t&...)> trigger = nullptr;

            // template <typename... values_t>
            // std::function<void(std::function<void(values_t&...)>)> on_trigger = nullptr;
        };

        struct runtime_curve_component {
        };

        struct runtime_widget_component {
            std::unique_ptr<std::type_info> typeinfo = nullptr;
            std::any untyped = nullptr;
            runtime_widget_data data;
        };

        struct context_impl {

            // inline static ecs::registry events_container;
            // inline static ecs::registry curves_container;
            // inline static ecs::registry animations_container;
            // inline static ecs::registry styles_container;
            inline static ecs::registry widgets_container;

            inline static std::unordered_map<id::integer, runtime_widget_component*> widgets_ptrs_staging_container;
            inline static std::tree<runtime_widget_component*> widgets_ptrs_container;

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