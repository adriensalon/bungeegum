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
            inline static context& get_context()
            {
                static context _context;
                return _context;
            }
        };
    }
}

context::context() { }

context& get_context()
{
    return internal::impl::context_manager::get_context();
}

}