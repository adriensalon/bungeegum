//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/simd.hpp>

namespace staticgui {
namespace detail {

    struct resolve_constraint_data {
        simd_array<float, 2> min_size;
        simd_array<float, 2> max_size;
    };

    struct resolve_command_data {
        resolve_constraint_data constraint;
        simd_array<float, 2> resolved_size;
    };

}
}
