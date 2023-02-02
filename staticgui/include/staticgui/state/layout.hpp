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

    struct constraint_data {
        bool is_tight;
        bool has_infinite_width;
        bool has_infinite_height;
        glue::simd_array<float, 2> min_size;
        glue::simd_array<float, 2> max_size;
    };

    struct geometry_data {
        glue::simd_array<float, 2> size;
    };

    // implements https://api.flutter.dev/flutter/rendering/RenderObject/parentData.html
}
}
