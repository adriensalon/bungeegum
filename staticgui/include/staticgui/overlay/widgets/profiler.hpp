//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/time.hpp>
#include <staticgui/state/context.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        void setup_profiler(context_state& context);

        void draw_profiler(context_state& context);
    }
}
}
