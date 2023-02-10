//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/context.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        struct shared_data {
            inline static unsigned int vertices_count = 0;
            inline static unsigned int indices_count = 0;
            inline static unsigned int commands_count = 0;
        };

        void setup_overlay(context_state& context);

        void draw_overlay(context_state& context, const std::function<void(ImDrawList*)>& draw_commands);
    }
}
}
