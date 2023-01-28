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
namespace internal {
    namespace impl {

        struct context_manager {
            inline static context immediate_context;
        };
    }
}

context::context() { }

// template <typename widget_t>
// context::destroy(widget_t& widget, const bool destroy_children)
// {
//     using namespace internal::impl;
//     // destroy component
//     // widgets_container.destroy()

//     // destroy in tree
//     // widgets_ptrs_container.
// }

context& get_context()
{
    return internal::impl::context_manager::immediate_context;
}

}